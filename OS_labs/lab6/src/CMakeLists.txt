cmake_minimum_required(VERSION 3.10)
project(lab6)

set(CMAKE_CXX_STANDARD 17)

add_executable(control_node control_node.cpp)
add_executable(compute_node compute_node.cpp)
add_library(functions functions.cpp functions.h)

target_link_libraries(functions zmq)
target_link_libraries(control_node zmq functions)
target_link_libraries(compute_node zmq functions)
