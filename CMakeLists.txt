# --------------------------------------------------------------------------------------------------
# ---- basic setup ---------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)
project(AoC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_subdirectory(cpp/aoc-cpp)

message("\n--------------------------------------------------------------------------------")
message("---- installing external libs --------------------------------------------------")
message("--------------------------------------------------------------------------------")

include(FetchContent)

message("---- GoogleTest ----------------------------------------------------------------")
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

message("---- raylib --------------------------------------------------------------------")
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

message("---- raylib C++ ----------------------------------------------------------------")
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

message("---- raygui --------------------------------------------------------------------")
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 4.0
)
FetchContent_MakeAvailable(raygui)

message("---- absl ----------------------------------------------------------------------")
set(ABSL_ENABLE_INSTALL TRUE) 
FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG 20240116.2
)
FetchContent_MakeAvailable(absl)

message("---- re2 -----------------------------------------------------------------------")
FetchContent_Declare(
    re2
    GIT_REPOSITORY https://github.com/google/re2.git
    GIT_TAG 2024-06-01
)
FetchContent_MakeAvailable(re2)

message("---- fmttools ------------------------------------------------------------------")
FetchContent_Declare(
        fmttools
        GIT_REPOSITORY https://github.com/awegsche/fmttools
        GIT_TAG 0.1.2
)

FETCHCONTENT_MAKEAVAILABLE(fmttools)

message("--------------------------------------------------------------------------------")
message("---- including subdirs ---------------------------------------------------------")
message("--------------------------------------------------------------------------------")
add_subdirectory(2015_c)

add_subdirectory(2022)

add_subdirectory(2019_cpp)

include_directories(aoc-cpp)

