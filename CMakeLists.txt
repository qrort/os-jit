cmake_minimum_required(VERSION 2.8)
project(os-find)

set(CMAKE_CXX_STANDARD 11)

add_compile_definitions(LOCAL)

add_compile_options(-O0)

add_executable(main main.cpp)