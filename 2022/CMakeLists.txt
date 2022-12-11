cmake_minimum_required(VERSION 3.24)
project(2022)

set(CMAKE_CXX_STANDARD 23)

include_directories(../aoc-cpp)
include_directories(../ext/re2/re2)

add_executable(2022
        day3.h
        main.cpp
         "day11.h")

target_link_libraries( 2022
        aoc_cpp
        re2
        )
