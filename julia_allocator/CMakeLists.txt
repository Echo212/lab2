# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A Win32 -B ../AllocatorBuild -S .
#
# *** Linux ***
# cmake -G "Unix Makefiles" -B ../AllocatorBuild -S .

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)
SET(CMAKE_CXX_FLAGS  "-g")

# Project name and language (C or C++)
project(PROJECT_JULIA_ALLOCATOR LANGUAGES CXX)

# Collect all .cpp and *.h source files in the current directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")

# Add an executable target
add_executable(julia_allocator ${SOURCES})



