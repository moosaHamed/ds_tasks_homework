#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include <list>
#include <limits>
#include <cassert>
#include "tasks.h"
#include "utilities.h"
#include "product.h"

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

// The following template method returns a pointer to maximum number from a contiguous memory buffer using c++11.
template<typename T>
T *ptr_to_maximum(const buffer<T> *buffer) {
    if (buffer == nullptr || buffer->values[0] == NULL) return nullptr;
    assert(buffer->size >= 2 && buffer->size <= 9999);
    int *max_ptr = buffer->values;
    for (int i = 1; i < buffer->size; ++i) {
        if (buffer->values[i] > *max_ptr) *max_ptr = buffer->values[i];
        else continue;
    }
    return max_ptr;
}

// The following method returns all factor(s) for a 3 digit number and stores it in contiguous memory buffer using c++11.
std::tuple<int, buffer<int>, std::string> get_a_three_digit_number_factors(const int integer) {
    int count = 0;
    buffer<int> _buffer = buffer_alloc<int>(FIXED_BUFFER_SIZE);
    if (integer > 0 && integer <= 999) {
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
    } else return std::make_tuple(0, _buffer, "The number must be from 1 to 999 to get its factors");

//  for security purpose and avoiding memory leak
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