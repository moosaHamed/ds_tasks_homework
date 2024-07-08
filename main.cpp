#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <map>

static const int FIXED_BUFFER_SIZE = 8;
static const int MAX_TRIAL_ENTERS = 16;

// Generic template type for allocating a contiguous chunk of memory
template<typename T>
struct buffer {
    int size;
    T *values;
};

// Generic template method for allocating a contiguous chunk of memory
template<typename T>
buffer<T> buffer_alloc(const int size) {
    return {size, (T *) malloc(sizeof(T) * size)};
}

// Generic template method for reallocating a contiguous chunk of memory
template<typename T>
buffer<T> buffer_realloc(buffer<T> &buffer, int new_size) {
    assert(new_size != buffer.size);
    buffer.size = new_size;
    T *result = (T *) realloc(buffer.values, sizeof(T) * new_size);
    if (result) buffer.values = result;
    else std::cout << "fails to realloc the buffer";
    return buffer;
}

// Generic template method for printing a contiguous chunk of memory in console
template<typename T>
void print_buffer(const buffer<T> &buffer) {
    if (buffer.size < 1) return;
    else if (buffer.size == 1) std::cout << "[" << buffer.values[0] << "]" << std::endl;
    else {
        std::cout << "[";
        for (int i = 0; i < buffer.size - 1; ++i) std::cout << buffer.values[i] << ", ";
        std::cout << buffer.values[buffer.size - 1] << "]" << std::endl;
    }
}

/* Method used for checking if entered character(s) is/are number and return a boolean either true if a number or false if not.
 * This method is used in the following three methods that used (cin) to accept a limited amount of character(s) a time.
*/
bool as_number(const buffer<char> *buffer) {
    bool flag = false;
    if (buffer->values[0] == '-') {
        for (int i = 1; i < FIXED_BUFFER_SIZE; ++i)
            if (buffer->values[i] >= '0' && buffer->values[i] <= '9') flag = true;
            else if (buffer->values[i] == '\0') break;
            else {
                flag = false;
                break;
            }
    } else
        for (int i = 0; i < FIXED_BUFFER_SIZE; ++i) {
            if (buffer->values[i] >= '0' && buffer->values[i] <= '9') flag = true;
            else if (buffer->values[i] == '\0') break;
            else {
                flag = false;
                break;
            }
        }
    return flag;
}

/*
 * Following three methods used to accept user input from keyboard, and it used (cin) for that purpose. However, the methods designed to
 * limit character numbers that a user can enter at a time specified as "FIXED_BUFFER_SIZE" and limit user trials to enter number(s)
 * specified as "MAX_TRIAL_ENTERS". This customization will improve the design of the task. Since user inputs cannot be predictable,
 * limiting the characters entered will enhance the security of code and then cast entered characters to desired types. Minimizing the
 * trials that user can enter a number will control the scope of the task running time and not make it open to exhaust available resources.
*/

// Method #1 to accept user input through keyboard and stored in fixed size buffer specified prior process start.
buffer<int> user_enter_fixed_size_numbers(int size) {
    assert(size < MAX_TRIAL_ENTERS);
    buffer<char> char_buffer = buffer_alloc<char>(FIXED_BUFFER_SIZE);
    buffer<int> int_buffer = buffer_alloc<int>(size);
    int_buffer.size = 0;
    int trial_count = 0;
    std::cout << "Kindly, Enter " << size << " Number(s):" << std::endl;
    std::cout << "Note: Process will terminate once hitting max trials" << std::endl;
    for (int i = 0; i < size; ++i) {
        if (trial_count < MAX_TRIAL_ENTERS) {
            std::cout << "Enter Number #" << i + 1 << ": ";
            // specifying input length before using (cin)
            std::cin.width(FIXED_BUFFER_SIZE);
            std::cin >> char_buffer.values;
            trial_count++;
            char_buffer.values[FIXED_BUFFER_SIZE - 1] = '\0';
            if (as_number(&char_buffer)) {
                std::cout << "You Entered the number #" << i + 1 << ": " << char_buffer.values << std::endl;
                int_buffer.size++;
                int_buffer.values[i] = std::stoi(char_buffer.values);
            } else {
                std::cout << "Not Valid Number, please try again" << std::endl;
                i--;
            }
            // to ignore multiple entered numbers seperated by spaces by user
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "Sorry, Process Terminated. You reached the Maximum Trials!.." << std::endl;
            break;
        }

    }
    return int_buffer;
}

/*
 * Method #2 to accept user input through keyboard and stored in fixed size buffer without specifying the buffer size. The Process will terminate either
 * if a user hits the max trials allowed or once predicate is true. The output buffer will reallocate a new chunk of "FIXED_BUFFER_SIZE" if predicate is not
 * equal to true. However, the process will terminate if the user hits the max trail even if predicate is not equal to true.
*/
buffer<int> user_enter_numbers_until_predicate() {
    buffer<char> char_buffer = buffer_alloc<char>(FIXED_BUFFER_SIZE);
    buffer<int> int_buffer = buffer_alloc<int>(MAX_TRIAL_ENTERS / 2);
    int_buffer.size = 0;
    int trial_count = 0;
    std::cout << "Kindly, Enter some Number(s):" << std::endl;
    std::cout << "Note: Process will terminate if number is negative or once hitting max trials" << std::endl;
    for (int i = 0; i < MAX_TRIAL_ENTERS; ++i) {
        if (trial_count < MAX_TRIAL_ENTERS) {
            if (int_buffer.size <= i) buffer_realloc(int_buffer, int_buffer.size + MAX_TRIAL_ENTERS / 2);
            std::cout << "Enter Number #" << i + 1 << ": ";
            // specifying input length before using (cin)
            std::cin.width(FIXED_BUFFER_SIZE);
            std::cin >> char_buffer.values;
            trial_count++;
            char_buffer.values[FIXED_BUFFER_SIZE - 1] = '\0';
            if (as_number(&char_buffer)) {
                std::cout << "You Entered the number #" << i + 1 << ": " << char_buffer.values << std::endl;
                int n = std::stoi(char_buffer.values);
                int_buffer.size++;
                int_buffer.values[i] = n;
                if (n < 0) {
                    std::cout << "You entered negative number: " << n << " ,Process Terminated!" << std::endl;
                    break;
                }
            } else {
                std::cout << "Not Valid Number, please try again" << std::endl;
                i--;
            }
            // to ignore multiple entered numbers seperated by spaces by user
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "Sorry, Process Terminated. You reached the Maximum Trials!.." << std::endl;
            break;
        }
    }
    return int_buffer;
}

/*
 * Method #3 to accept user input through keyboard and stored in single int type in buffer. The Process will terminate either if a user hits the max
 * trials allowed or once a single int type is entered.
*/
int *user_enter_a_number() {
    buffer<char> char_buffer = buffer_alloc<char>(FIXED_BUFFER_SIZE);
    int *n = (int *) malloc(sizeof(int));
    int trial_count = 0;
    for (int i = 0; i < MAX_TRIAL_ENTERS - 1; ++i) {
        std::cout << "Enter a number: ";
        std::cout << "Note: Process will terminate if you enter a number or once hitting max trials" << std::endl;
        // specifying input length before using (cin)
        std::cin.width(FIXED_BUFFER_SIZE);
        std::cin >> char_buffer.values;
        trial_count++;
        char_buffer.values[FIXED_BUFFER_SIZE - 1] = '\0';
        if (as_number(&char_buffer)) {
            std::cout << "You Entered the number: " << char_buffer.values << std::endl;
            *n = std::stoi(char_buffer.values);
            break;
        } else {
            if (trial_count == MAX_TRIAL_ENTERS - 1) {
                std::cout << "Sorry, Process Terminated. You reached the Maximum Trials!.." << std::endl;
                n = nullptr;
                break;
            } else {
                std::cout << "Not Valid Number, please try again" << std::endl;
            }
        }
        // to ignore multiple entered numbers seperated by spaces by user
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return n;
}

typedef enum {
    Percent_20,
    Percent_10,
    No_Discount
} Discount;

std::string discount_message(Discount discount) {
    switch (discount) {
        case Percent_20:
            return "20% Discount Applied";
        case Percent_10:
            return "10% Discount Applied";
        case No_Discount:
            return "-";
    }
}

std::string stringify_discount(Discount discount) {
    switch (discount) {
        case Percent_20:
            return "20%";
        case Percent_10:
            return "10%";
        case No_Discount:
            return "-";
    }
}

typedef struct {
    int product_id;
    int requestedQty;
    double totalPrice;
    std::string discount;
    std::string message;
    double totalAfterPrice;
} MakePurchase;

class Product {
public:
    int Id;
    std::string ProductName;
    double UnitPrice;
    int AvailableStock;

    constexpr static const double DISCOUNT_10_PERCENT_FACTOR = 0.9;
    constexpr static const double DISCOUNT_20_PERCENT_FACTOR = 0.8;

    Product(int id, const std::string &productName, double unitPrice, int initialStock) {
        Id = id;
        ProductName = productName;
        UnitPrice = unitPrice;
        AvailableStock = initialStock;
    }

    double get_price(int requested_qty) const {
        if (requested_qty < 1) return 0.0;
        else return this->UnitPrice * requested_qty;
    }

    MakePurchase make_purchase(int requested_qty) {
        if (requested_qty < 1)
            return {this->Id, requested_qty, 0.0, stringify_discount(No_Discount), "Requested Qty must be at least 1 Piece.", 0.0};
        else if (requested_qty >= 10 && requested_qty <= 99 && requested_qty <= this->AvailableStock) {
            this->AvailableStock = this->AvailableStock - requested_qty;
            return {this->Id, requested_qty, get_price(requested_qty), stringify_discount(Percent_10),
                    discount_message(Percent_10), get_price(requested_qty) * DISCOUNT_10_PERCENT_FACTOR};
        } else if (requested_qty >= 100 && requested_qty <= this->AvailableStock) {
            this->AvailableStock = this->AvailableStock - requested_qty;
            return {this->Id, requested_qty, get_price(requested_qty), stringify_discount(Percent_20),
                    discount_message(Percent_20), get_price(requested_qty) * DISCOUNT_20_PERCENT_FACTOR};
        } else if (requested_qty < 10 && requested_qty <= this->AvailableStock) {
            this->AvailableStock = this->AvailableStock - requested_qty;
            return {this->Id, requested_qty, get_price(requested_qty), stringify_discount(No_Discount),
                    discount_message(No_Discount), get_price(requested_qty)};
        } else if (requested_qty > this->AvailableStock)
            return {this->Id, requested_qty, 0.0, stringify_discount(No_Discount),
                    "Requested Qty is higher than Available Stock. Consider lowering it.", 0.0};
        else return {0, requested_qty, 0, stringify_discount(No_Discount), "Error. Can not be processed!.", 0.0};
    }
};

class ProductRepository {
private:
    const int MAX_PRODUCT_NAME_SIZE = 40;
    const int MAX_PRODUCTS_COUNT_IN_REPO = 6;
    const int MAX_STOCK_ALLOWED_IN_REPO = 512;

public:
    int occupied_slots;
    buffer<Product> _buffer;

    ProductRepository() {
        _buffer = buffer_alloc<Product>(MAX_PRODUCTS_COUNT_IN_REPO);
        occupied_slots = 0;
    }

    void add_a_product_to_repo(const std::string &ProductName, double UnitPrice, int InitialQuantity) {
        if (occupied_slots == MAX_PRODUCTS_COUNT_IN_REPO) return;
        for (int i = 0; i < occupied_slots; ++i)
            if (_buffer.values[i].ProductName == ProductName) return;
            else continue;
        if (ProductName.size() <= MAX_PRODUCT_NAME_SIZE && InitialQuantity <= MAX_STOCK_ALLOWED_IN_REPO && InitialQuantity > 0) {
            _buffer.values[occupied_slots] = Product(occupied_slots + 1, ProductName, UnitPrice, InitialQuantity);
            occupied_slots++;
        }
    }

    MakePurchase purchase_a_product(int id, int requested_qty) const {
        MakePurchase makePurchase;
        for (int i = 0; i < _buffer.size; ++i) {
            if (_buffer.values[i].Id == id) {
                makePurchase = _buffer.values[i].make_purchase(requested_qty);
                break;
            } else if (i == _buffer.size - 1 && _buffer.values[i].Id != id) {
                makePurchase = {id, requested_qty, 0, stringify_discount(No_Discount), "Purchase Fail. Product with entered Id not found!.", 0.0};
            } else continue;
        }
        return makePurchase;
    }

    void print_purchase_product(MakePurchase &makePurchase) const {
        for (int i = 0; i < occupied_slots; ++i) {
            if (_buffer.values[i].Id == makePurchase.product_id) {
                std::cout
                        << "______________________________________________________________________________________________________________________________________\n";
                std::cout
                        << "| You purchased the following:                                                                                                       |\n";
                std::cout
                        << "--------------------------------------------------------------------------------------------------------------------------------------\n";
                std::cout
                        << "| ID | Product Name                            | Qty. | Unit Price (OMR) | Total Price (OMR) | Discount | Total After Discount (OMR) |\n";
                std::cout
                        << "--------------------------------------------------------------------------------------------------------------------------------------\n";
                std::cout << "| " + std::to_string(_buffer.values[i].Id) + std::string(3 - std::to_string(_buffer.values[i].Id).size(), ' ') + "| "
                             + _buffer.values[i].ProductName + std::string(40 - _buffer.values[i].ProductName.size(), ' ') + "| "
                             + std::to_string(makePurchase.requestedQty) + std::string(5 - std::to_string(makePurchase.requestedQty).size(), ' ') + "| "
                             + std::to_string(_buffer.values[i].UnitPrice) + std::string(17 - std::to_string(_buffer.values[i].UnitPrice).size(), ' ') + "| "
                             + std::to_string(makePurchase.totalPrice) + std::string(18 - std::to_string(makePurchase.totalPrice).size(), ' ') + "| "
                             + makePurchase.discount + std::string(9 - makePurchase.discount.size(), ' ') + "| " + std::to_string(makePurchase.totalAfterPrice)
                             + std::string(27 - std::to_string(makePurchase.totalAfterPrice).size(), ' ') + "|\n";
                std::cout
                        << "--------------------------------------------------------------------------------------------------------------------------------------\n";
                break;
            } else if (i == occupied_slots - 1 && _buffer.values[i].Id != makePurchase.product_id) {
                std::cout << makePurchase.message << std::endl;
            } else continue;
        }
    }

    void print_repo_products() const {
        if (occupied_slots == 0) return;
        std::cout << "_____________________________________________________________________________________\n";
        std::cout << "| Available Products in Store:                                                      |\n";
        std::cout << "-------------------------------------------------------------------------------------\n";
        std::cout << "| ID | Product Name                            | Available Stock | Unit Price (OMR) |\n";
        std::cout << "-------------------------------------------------------------------------------------\n";
        for (int i = 0; i < occupied_slots; ++i) {
            std::cout << "| " + std::to_string(_buffer.values[i].Id) + std::string(3 - std::to_string(_buffer.values[i].Id).size(), ' ') + "| "
                         + _buffer.values[i].ProductName + std::string(40 - _buffer.values[i].ProductName.size(), ' ') + "| "
                         + std::to_string(_buffer.values[i].AvailableStock) + std::string(16 - std::to_string(_buffer.values[i].AvailableStock).size(), ' ') +
                         "| "
                         + std::to_string(_buffer.values[i].UnitPrice) + std::string(17 - std::to_string(_buffer.values[i].UnitPrice).size(), ' ') + "|\n";
            std::cout << "-------------------------------------------------------------------------------------\n";
        }
    }

};

// The following template method returns a pointer to maximum number from a contiguous memory buffer using c++11.
template<typename T>
T *ptr_to_maximum(const buffer<T> *buffer) {
    if (buffer == nullptr) return nullptr;
    if (buffer->size == 0) return nullptr;
    assert(buffer->size >= 2 && buffer->size <= 9999);
    T *max_ptr = buffer->values;
    for (int i = 1; i < buffer->size; ++i) {
        if (buffer->values[i] > *max_ptr) *max_ptr = buffer->values[i];
        else continue;
    }
    return max_ptr;
}

// The following method returns all factor(s) for a 6 digits number and stores it in contiguous memory buffer using c++11.
std::tuple<int, buffer<int>, std::string> get_a_three_digit_number_factors(const int integer) {
    int count = 0;
    buffer<int> _buffer = buffer_alloc<int>(FIXED_BUFFER_SIZE);
    if (integer > 0 && integer <= 999999) {
        for (int i = 1; i <= integer; ++i) {
            if (integer % i == 0 && _buffer.size <= count) {
                buffer_realloc(_buffer, _buffer.size + FIXED_BUFFER_SIZE);
                _buffer.values[count] = i;
                count++;
            } else if (integer % i == 0 && _buffer.size > count) {
                _buffer.values[count] = i;
                count++;
            } else continue;
        }
    } else return std::make_tuple(0, _buffer, "The number must be from 1 to 999999 to get its factors");

//  for security purpose and avoiding memory leak, realloc and reduce buffer size to match count
    if (count != _buffer.size) {
        buffer_realloc(_buffer, count);
    }
    return std::make_tuple(count, _buffer, "The are/is " + std::to_string(count) + " factor(s) for the number " + std::to_string(integer) + " :");
}

// The following method splits Even/Odd numbers asides in contiguous memory buffer using c++11.
buffer<int> split_odd_even_asides(const buffer<int> &int_buffer) {
    assert(int_buffer.size >= 2);
    buffer<int> buffer = buffer_alloc<int>(int_buffer.size);
    int *pv = buffer.values + (int_buffer.size - 1);
    int odd = 0;
    int even = 0;
    for (int i = 0; i < int_buffer.size; ++i) {
        if (int_buffer.values[i] % 2 != 0) {
            *(buffer.values + odd) = int_buffer.values[i];
            odd++;
        } else if (int_buffer.values[i] % 2 == 0) {
            *(pv - even) = int_buffer.values[i];
            even++;
        } else continue;
    }
    return buffer;
}

// The following method demonstrates the creation of new std::stack in c++11 and using it to get the maximum in contiguous memory buffer.
int get_maximum_using_stack(const buffer<int> &buffer) {
    int maximum;
    std::stack<int> stack;
    for (int i = 0; i < buffer.size; ++i) stack.push(buffer.values[i]);
    maximum = stack.top();
    for (int i = 1; i < buffer.size; ++i) {
        stack.pop();
        if (stack.top() > maximum) maximum = stack.top();
    }
    return maximum;
}

// The following method demonstrates the creation of new std::queue in c++11 and using it to get smallest in contiguous memory buffer.
int get_smallest_using_queue(const buffer<int> &buffer) {
    int smallest;
    std::queue<int> queue;
    for (int i = 0; i < buffer.size; ++i) queue.push(buffer.values[i]);
    smallest = queue.front();
    for (int i = 1; i < buffer.size; ++i) {
        queue.pop();
        if (queue.front() < smallest) smallest = queue.front();
    }
    return smallest;
}

// The following method demonstrates the creation of new std::map in c++11 and iteration on it to print it and find an item on it.
void store_movies_in_map() {
    //declare variables & Store data in the map
    std::map<int, std::string> movies_map{{1, "Ghosted"},
                                          {2, "Encanto"},
                                          {3, "Succession"},
                                          {4, "Titanic"}};
    // display all ID and names
    std::cout << "Movie ID" << "   " << "Movie Name" << std::endl;
    for (const auto &m: movies_map)
        std::cout << m.first << "          " << m.second << std::endl;
    // display the IDs of movies which name starts with ‘S’
    std::cout << std::endl;
    std::cout << "Movie(s) start with letter ‘S’:" << std::endl;
    std::cout << "Movie ID" << "   " << "Movie Name" << std::endl;
    for (const auto &m: movies_map)
        if (!m.second.rfind('S', 0))
            std::cout << m.first << "          " << m.second << std::endl;

}

void product_class_demo() {
    std::map<std::string, double> row_data{{"2\" BRASS GATE VALVE",                       11.8},
                                           {"1\" uPVC H.P.ELBOW 90DEG S/W FIP",           0.2995},
                                           {"1\" uPVC H.P.END CAP S/W FIP",               0.19233},
                                           {"1\" x 3/4\" uPVC H.P.REDUCING BUSH S/W FIP", 0.1925}};
    ProductRepository productRepository = ProductRepository();
    for (int i = 0; i < row_data.size(); ++i) {
        auto it = row_data.begin();
        std::advance(it, i);
        productRepository.add_a_product_to_repo(it->first, it->second, 500);
    }
    productRepository.print_repo_products();
    std::cout << std::endl;
    MakePurchase makePurchase = productRepository.purchase_a_product(4, 400);
    productRepository.print_purchase_product(makePurchase);
    std::cout << std::endl;
    productRepository.print_repo_products();
    free(productRepository._buffer.values);
}

int main() {

    /**********************************************************/
    /** Comments all Tasks and Run Only One Task Per Process **/
    /**********************************************************/

    /** Task #1: a) User enters 3 numbers and display the maximum one **/
    buffer<int> _buffer = user_enter_fixed_size_numbers(3);
    if (_buffer.values && _buffer.size == 3) {
        int *max = ptr_to_maximum(&_buffer);
        std::cout << std::endl;
        std::cout << "The Maximum Number is: " << *max << std::endl;
        free(_buffer.values);
    } else {
        if (_buffer.values && _buffer.size > 0) free(_buffer.values);
    }

    /** Task #1: b) User enters numbers until predicate and display the maximum one **/
    buffer<int> _buffer_01 = user_enter_numbers_until_predicate();
    if (_buffer_01.values && _buffer_01.size > 0) {
        int *max = ptr_to_maximum(&_buffer_01);
        std::cout << std::endl;
        std::cout << "The Maximum Number is: " << *max << std::endl;
        free(_buffer_01.values);
    }

    /** Task #1: c) Product class **/
    product_class_demo();

    /** Task #2: Print all factors of a number **/
    std::tuple<int, buffer<int>, std::string> tuple_1 = get_a_three_digit_number_factors(84862);
    if (std::get<0>(tuple_1) > 0) {
        buffer<int> buffer = std::get<1>(tuple_1);
        std::cout << std::get<2>(tuple_1);
        print_buffer(buffer);
    }

    /** Task #3: Get pointer to maximum double from arrays of doubles **/
    double array[] = {-100.0, 2.0, 4.0, 15.9, 30.9, 33.445, 7.99, 11.3};
    buffer<double> _buffer_02 = buffer_alloc<double>(FIXED_BUFFER_SIZE);
    for (int i = 0; i < FIXED_BUFFER_SIZE; ++i) {
        _buffer_02.values[i] = array[i];
        _buffer_02.size = _buffer_02.size + i;
    }
    if (_buffer_02.values && _buffer_02.size > 0) {
        double *max = ptr_to_maximum(&_buffer_02);
        std::cout << std::endl;
        std::cout << "The Maximum Number is: " << *max << std::endl;
        free(_buffer_02.values);
    }

    /** Task #4: User enters several integers and store it in array then split odd ones to left and evens to right **/
    buffer<int> _buffer_03 = user_enter_fixed_size_numbers(MAX_TRIAL_ENTERS-1);
    if (_buffer_03.values && _buffer_03.size > 1) {
        buffer<int> _buffer_04 = split_odd_even_asides(_buffer_03);
        std::cout << std::endl;
        std::cout << "Original numbers List: ";
        print_buffer(_buffer_03);
        std::cout << "Split Odd/Even List: ";
        print_buffer(_buffer_04);
        free(_buffer_03.values);
        free(_buffer_04.values);
    } else {
        if (_buffer_03.values && _buffer_03.size > 0) free(_buffer_03.values);
    }

    /** Task #4: User enters several integers and store it in array then using stack to return the maximum one **/
    buffer<int> _buffer_05 = user_enter_fixed_size_numbers(MAX_TRIAL_ENTERS - 1);
    if (_buffer_05.values && _buffer_05.size > 1) {
        std::cout << std::endl;
        int max = get_maximum_using_stack(_buffer_05);
        std::cout << "Original numbers List: ";
        print_buffer(_buffer_05);
        std::cout << "The Maximum Number is: " << max << std::endl;
    } else {
        if (_buffer_05.values && _buffer_05.size > 0) free(_buffer_05.values);
    }

    /** Task #4: User enters several integers and store it in array then using queue to return the smallest one **/
    buffer<int> _buffer_06 = user_enter_fixed_size_numbers(MAX_TRIAL_ENTERS - 1);
    if (_buffer_06.values && _buffer_06.size > 1) {
        std::cout << std::endl;
        int Smallest = get_smallest_using_queue(_buffer_06);
        std::cout << "Original numbers List: ";
        print_buffer(_buffer_06);
        std::cout << "The Smallest Number is: " << Smallest << std::endl;
    } else {
        if (_buffer_06.values && _buffer_06.size > 0) free(_buffer_06.values);
    }

    /** Task #9: Using map to store and display some movies **/
    store_movies_in_map();

    return 0;

}
