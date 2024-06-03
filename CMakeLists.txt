cmake_minimum_required(VERSION 3.24)
project(AoC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_subdirectory(cpp/aoc-cpp)
add_subdirectory(ext/re2)
add_subdirectory(ext/glm)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

include(FetchContent)

FetchContent_Declare(
        fmttools
        GIT_REPOSITORY https://github.com/awegsche/fmttools
        GIT_TAG 0.1.2
)

FETCHCONTENT_MAKEAVAILABLE(fmttools)

add_subdirectory(2022)
add_subdirectory(2019_cpp)

include_directories(aoc-cpp)

