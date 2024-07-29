cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(imgui_test VERSION 0.0.1 DESCRIPTION "testing imgui" LANGUAGES C CXX)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)

add_subdirectory(src)

target_compile_options(imgui_test PUBLIC -FC -std:c++20)
