#include <iomanip>
#include "product.h"
#include "utilities.h"

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

Product::Product(int id, const std::string &productName, double unitPrice, int initialStock) {
    Id = id;
    ProductName = productName;
    UnitPrice = unitPrice;
    AvailableStock = initialStock;
}

ProductRepository::ProductRepository() {
    _buffer = buffer_alloc<Product>(MAX_PRODUCTS_COUNT_IN_REPO);
    occupied_slots = 0;
}


double Product::get_price(int requested_qty) const {
    if (requested_qty < 1) return 0.0;
    else return this->UnitPrice * requested_qty;
}

MakePurchase Product::make_purchase(int requested_qty) {
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

void ProductRepository::add_a_product_to_repo(const std::string &ProductName, double UnitPrice, int InitialQuantity) {
    if (occupied_slots == MAX_PRODUCTS_COUNT_IN_REPO) return;
    for (int i = 0; i < occupied_slots; ++i)
        if (_buffer.values[i].ProductName == ProductName) return;
        else continue;
    if (ProductName.size() <= MAX_PRODUCT_NAME_SIZE && InitialQuantity <= MAX_STOCK_ALLOWED_IN_REPO && InitialQuantity > 0) {
        _buffer.values[occupied_slots] = Product(occupied_slots + 1, ProductName, UnitPrice, InitialQuantity);
        occupied_slots++;
    }
}

MakePurchase ProductRepository::purchase_a_product(int id, int requested_qty) const {
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

void ProductRepository::print_purchase_product(MakePurchase &makePurchase) const {
    for (int i = 0; i < occupied_slots; ++i) {
        if (_buffer.values[i].Id == makePurchase.product_id) {
            std::cout << "______________________________________________________________________________________________________________________________________\n";
            std::cout << "| You purchased the following:                                                                                                       |\n";
            std::cout << "--------------------------------------------------------------------------------------------------------------------------------------\n";
            std::cout << "| ID | Product Name                            | Qty. | Unit Price (OMR) | Total Price (OMR) | Discount | Total After Discount (OMR) |\n";
            std::cout << "--------------------------------------------------------------------------------------------------------------------------------------\n";
            std::cout << "| " + std::to_string(_buffer.values[i].Id) + std::string(3 - std::to_string(_buffer.values[i].Id).size(), ' ') + "| "
                         + _buffer.values[i].ProductName + std::string(40 - _buffer.values[i].ProductName.size(), ' ') + "| "
                         + std::to_string(makePurchase.requestedQty) + std::string(5 - std::to_string(makePurchase.requestedQty).size(), ' ') + "| "
                         + std::to_string(_buffer.values[i].UnitPrice) + std::string(17 - std::to_string(_buffer.values[i].UnitPrice).size(), ' ') + "| "
                         + std::to_string(makePurchase.totalPrice) + std::string(18 - std::to_string(makePurchase.totalPrice).size(), ' ') + "| "
                         + makePurchase.discount + std::string(9 - makePurchase.discount.size(), ' ') + "| " + std::to_string(makePurchase.totalAfterPrice)
                         + std::string(27 - std::to_string(makePurchase.totalAfterPrice).size(), ' ') + "|\n";
            std::cout << "--------------------------------------------------------------------------------------------------------------------------------------\n";
            break;
        } else if (i == occupied_slots - 1 && _buffer.values[i].Id != makePurchase.product_id) {
            std::cout << makePurchase.message << std::endl;
        } else continue;
    }
}

void ProductRepository::print_repo_products() const {
    if (occupied_slots == 0) return;
    std::cout << "_____________________________________________________________________________________\n";
    std::cout << "| Available Products in Store:                                                      |\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "| ID | Product Name                            | Available Stock | Unit Price (OMR) |\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
    for (int i = 0; i < occupied_slots; ++i) {
        std::cout << "| " + std::to_string(_buffer.values[i].Id) + std::string(3 - std::to_string(_buffer.values[i].Id).size(), ' ') + "| "
                     + _buffer.values[i].ProductName + std::string(40 - _buffer.values[i].ProductName.size(), ' ') + "| "
                     + std::to_string(_buffer.values[i].AvailableStock) + std::string(16 - std::to_string(_buffer.values[i].AvailableStock).size(), ' ') + "| "
                     + std::to_string(_buffer.values[i].UnitPrice) + std::string(17 - std::to_string(_buffer.values[i].UnitPrice).size(), ' ') + "|\n";
        std::cout << "-------------------------------------------------------------------------------------\n";
    }
}


