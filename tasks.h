#ifndef DS_TASKS_HOMEWORK_TASKS_H
#define DS_TASKS_HOMEWORK_TASKS_H

#include "utilities.h"

buffer<int> user_enter_fixed_size_numbers(int size);

buffer<int> user_enter_numbers_until_predicate();

int *user_enter_a_number();

template<typename T>
T *ptr_to_maximum(const buffer<T> *buffer);

std::tuple<int, buffer<int>, std::string> get_a_three_digit_number_factors( int integer);
buffer<int> split_odd_even_asides(const buffer<int> &int_buffer);
int get_maximum_using_stack(const buffer<int> &buffer);
int get_smallest_using_queue(const buffer<int> &buffer);

#endif //DS_TASKS_HOMEWORK_TASKS_H
