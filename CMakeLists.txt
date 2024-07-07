cmake_minimum_required(VERSION 3.28)
project(ds_tasks_homework)

set(CMAKE_CXX_STANDARD 11)

add_executable(ds_tasks_homework main.cpp
        utilities.cpp
        utilities.h
        tasks.cpp
        tasks.h
        product.cpp)
