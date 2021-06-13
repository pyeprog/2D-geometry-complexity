cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(geos)

add_library(complexity complexity.hpp complexity.cpp)

target_include_directories(complexity PUBLIC .)
target_link_libraries(complexity PUBLIC GEOS::geos)

find_package(CLI11)

add_executable(gcpx main.cpp complexity)
target_link_libraries(gcpx PRIVATE complexity CLI11::CLI11)
