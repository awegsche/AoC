cmake_minimum_required(VERSION 3.24)

message("---- 2019 in C++ ---------------------------------------------------------------")
project(2019)

set(CMAKE_CXX_STANDARD 23)

include_directories(../cpp/aoc-cpp)
include_directories(${CMAKE_SOURCE_DIR})

#find_package(raylib_cpp REQUIRED)
#find_package(raylib REQUIRED)

add_executable(2019
        main.cpp
        )

target_link_libraries( 2019
        aoc_cpp
        re2
        raylib
        raylib_cpp
        fmttools
        )


enable_testing()

add_executable(2019_tests
        day10_test.cpp
)

target_link_libraries(2019_tests
        aoc_cpp
        raylib
        raylib_cpp
        fmttools
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(2019_tests)
