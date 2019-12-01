cmake_minimum_required(VERSION 3.10)
project(OS_lab4 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS -pthread)

add_executable(OS_lab4 main_v2.c)