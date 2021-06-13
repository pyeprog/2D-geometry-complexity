cmake_minimum_required(VERSION 3.19)
project(complexity_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(GTest)

add_executable(complexity_test test_complexity.cpp test_cost.cpp)

target_link_libraries(complexity_test PRIVATE complexity)
target_link_libraries(complexity_test PUBLIC GTest::gtest_main)
