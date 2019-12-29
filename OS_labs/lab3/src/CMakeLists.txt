cmake_minimum_required(VERSION 3.10)
project(OS_lab3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS -pthread)

add_executable(OS_lab3 main.c)