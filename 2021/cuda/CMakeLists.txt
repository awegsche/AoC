# CMakeList.txt : CMake project for cuda, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project ("cuda" LANGUAGES CXX CUDA)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

add_compile_definitions(INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../inputs")

#include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
find_package(CUDAToolkit REQUIRED)

set(CMAKE_CUDA_ARCHITECTURES 86)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)


find_package(glew REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_subdirectory("ext/dear_spdlog")

find_package(imgui CONFIG REQUIRED)


# Add source to this project's executable.
add_executable (day11 "day11/main.cpp"
"utils/cuda_helpers.h"
"utils/device.h"
"utils/cuda_helpers.cpp"
"utils/gui.h"
"utils/gui.cpp"
"utils/fpscounter.h"
"day11/kernel.h"
"day11/kernel.cu"
 "utils/gldisplay.h")

target_link_libraries(day11
	PRIVATE
	dear_spdlogger
	spdlog::spdlog
	fmt::fmt
	glfw
	GLEW::GLEW
	imgui::imgui
	CUDA::cudart
	CUDA::cuda_driver
)

# TODO: Add tests and install targets if needed.
set_target_properties(day11 PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET day11 PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)
