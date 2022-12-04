cmake_minimum_required(VERSION 3.24)
project(AoC)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(aoc-cpp)
add_subdirectory(2022)
add_subdirectory(2015)

include_directories(aoc-cpp)

