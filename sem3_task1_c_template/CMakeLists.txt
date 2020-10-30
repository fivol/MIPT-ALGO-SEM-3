cmake_minimum_required(VERSION 3.16)
project(sem3_task1_c_template)

set(CMAKE_CXX_STANDARD 20)

add_library(string_template STATIC StringTemplate.h StringTemplate.cpp)

add_executable(sem3_task1_c_template main.cpp)
target_link_libraries(sem3_task1_c_template string_template)