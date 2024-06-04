cmake_minimum_required(VERSION 3.12)


message("---- 2015 in C -----------------------------------------------------------------")
project(aoc2015_c)

# find_package(raylib REQUIRED)

add_executable(
    aoc2015_c
    main.c
    )

target_link_libraries(
    aoc2015_c
    raylib)
