#include "utilities.h"

template<typename T>
buffer<T> buffer_alloc(const int size) {
    return {size, (T *) malloc(sizeof(T) * size)};
}

template<typename T>
buffer<T> buffer_realloc(buffer<T> &buffer, int new_size) {
    assert(new_size != buffer.size);
    buffer.size = new_size;
    T *result = (T *) realloc(buffer.values, sizeof(T) * new_size);
    if (result) buffer.values = result;
    else std::cout << "fails to realloc the buffer";
    return buffer;
}

template<typename T>
void print_buffer(const buffer <T> &buffer) {
    assert(buffer.size >= 1);
    if (buffer.size == 1) std::cout << "[" << buffer.values[0] << "]" << std::endl;
    else {
        std::cout << "[";
        for (int i = 0; i < buffer.size - 1; ++i) std::cout << buffer.values[i] << ", ";
        std::cout << buffer.values[buffer.size - 1] << "]" << std::endl;
    }
}

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
