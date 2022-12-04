CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

project(aoc-cpp)

set(CMAKE_CXX_STANDARD 23)

add_library(aoc_cpp
        lib.cpp
        aoc_object.h
        )