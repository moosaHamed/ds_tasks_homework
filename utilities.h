#ifndef DS_TASKS_HOMEWORK_UTILITIES_H
#define DS_TASKS_HOMEWORK_UTILITIES_H

#include <iostream>

const int FIXED_BUFFER_SIZE = 8;
const int MAX_TRIAL_ENTERS = 16;

// Generic Template Type for a contiguous chunk of memory in heap
template<typename T>
struct buffer {
    int size;
    T *values;
};

// Generic method for allocating a contiguous chunk of memory in heap
template<typename T>
buffer<T> buffer_alloc(int size);

// Generic method for extend an allocation of a contiguous chunk of memory in heap
template<typename T>
buffer<T> buffer_realloc(buffer<T> &buffer, int new_size);

// Generic method for print an allocated contiguous chunk of memory in heap
template<typename T>
void print_buffer(const buffer<T> &buffer);

// Method for checking if entered data is a number
bool as_number(const buffer<char> *buffer);


#endif //DS_TASKS_HOMEWORK_UTILITIES_H
