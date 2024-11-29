set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(allocator_static_pool_proj)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
include_directories(${SOURCE_DIR})
add_executable(main_allocator ${SOURCES})
