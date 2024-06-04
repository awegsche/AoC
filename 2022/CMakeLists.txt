cmake_minimum_required(VERSION 3.24)
project(2022)

set(CMAKE_CXX_STANDARD 23)

include_directories(../cpp/aoc-cpp)
include_directories(../ext/re2/re2)
include_directories(../ext/glm)

#find_package(raylib_cpp REQUIRED)
#find_package(raylib REQUIRED)

add_executable(2022
        main.cpp
        ../2019_cpp/main.cpp
        ../2019_cpp/day10.h)

target_link_libraries( 2022
        aoc_cpp
        re2
        )

add_executable(2022_gui
        main.cpp )

set_target_properties(2022
        PROPERTIES COMPILE_DEFINITIONS "NGUI"
)

target_link_libraries( 2022_gui
        aoc_cpp
        re2
        raylib
        raylib_cpp
        glm
        )
