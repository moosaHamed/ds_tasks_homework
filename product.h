#ifndef DS_2_PRODUCT_H
#define DS_2_PRODUCT_H

#include <string>
#include <iostream>

typedef enum {
    Percent_20,
    Percent_10,
    No_Discount
} Discount;

std::string stringify_discount(Discount discount);

std::string discount_message(Discount discount);

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

    Product(int id, const std::string &productName, double unitPrice, int initialStock);

    constexpr static const double DISCOUNT_10_PERCENT_FACTOR = 0.9;
    constexpr static const double DISCOUNT_20_PERCENT_FACTOR = 0.8;

    double get_price(int requested_qty) const;

    MakePurchase make_purchase(int requested_qty);
};


class ProductRepository {
private:
    const int MAX_PRODUCT_NAME_SIZE = 40;
    const int MAX_PRODUCTS_COUNT_IN_REPO = 6;
    const int MAX_STOCK_ALLOWED_IN_REPO = 512;

public:
    int occupied_slots;
    buffer<Product> _buffer;

    ProductRepository();

    void add_a_product_to_repo(const std::string &ProductName, double UnitPrice, int InitialQuantity);

    MakePurchase purchase_a_product(int id, int requested_qty) const;

    void print_purchase_product(MakePurchase &makePurchase) const;

    void print_repo_products() const;

};


#endif //DS_2_PRODUCT_H
