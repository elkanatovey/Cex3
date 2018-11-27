cmake_minimum_required(VERSION 3.12)
project(ex3 C)

set(CMAKE_C_STANDARD 99)

add_library(stack stack.h stack.c)
add_executable(ex3 calc.c)
target_link_libraries(ex3 m)
target_link_libraries(ex3 stack)