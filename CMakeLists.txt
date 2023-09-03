cmake_minimum_required(VERSION 3.26)
project(algos)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(algos template.h test.cpp)
