#include <iostream>
#include <map>
#include "product.h"

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

    return 0;

    /**********************************************************/
    /** Comments all Tasks and Run Only One Task Per Process **/
    /**********************************************************/

//
//    /** Task #1: a) User enters 3 numbers and display the maximum one **/
//    buffer<int> _buffer = user_enter_fixed_size_numbers(3);
//    if (_buffer.values && _buffer.size == 3) {
//        int *max = ptr_to_maximum(&_buffer);
//        std::cout << std::endl;
//        std::cout << "The Maximum Number is: " << *max << std::endl;
//        free(_buffer.values);
//    } else {
//        if (_buffer.values && _buffer.size > 0) free(_buffer.values);
//    }
//
//
//    /** Task #1: b) User enters numbers until predicate and display the maximum one **/
//    buffer<int> _buffer_01 = user_enter_numbers_until_predicate();
//    if (_buffer_01.values && _buffer_01.size > 0) {
//        int *max = ptr_to_maximum(&_buffer_01);
//        std::cout << std::endl;
//        std::cout << "The Maximum Number is: " << *max << std::endl;
//        free(_buffer_01.values);
//    }

    /** Task #1: c) Product class **/
//    product_class_demo();

//    /** Task #2: Print all factors of a number **/
//    std::tuple<int, buffer<int>, std::string> tuple = get_a_three_digit_number_factors(64);
//    if (std::get<0>(tuple) > 0) {
//        buffer<int> buffer = std::get<1>(tuple);
//        std::cout << std::get<2>(tuple);
//        print_buffer(buffer);
//    }

// Ptr to max double

// even odd splits

// stack the biggest number

// queue the smallest number

// The map question

}