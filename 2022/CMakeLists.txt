cmake_minimum_required(VERSION 3.24)
project(2022)

set(CMAKE_CXX_STANDARD 23)

include_directories(../aoc-cpp)

add_executable(2022 main.cpp)

target_link_libraries( 2022
        aoc_cpp)
