cmake_minimum_required(VERSION 3.27)
project(RVM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RVM_COMPILE_OPTIONS "-Wno-terminate")

enable_testing()

add_subdirectory(RVM)
add_subdirectory(tests)