CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

project(aoc-cpp)

set(CMAKE_CXX_STANDARD 23)

add_library(aoc_cpp
        timing.cpp
        lib.cpp
        Pos2D.cpp
        AocLines.cpp
)

target_link_libraries(aoc_cpp
        fmttools
)
