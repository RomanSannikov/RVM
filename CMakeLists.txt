cmake_minimum_required(VERSION 3.27)
project(RVM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RVM_COMPILE_OPTIONS "-Wall")
set(RVM_COMPILE_OPTIONS "-Wextra")
set(RVM_COMPILE_OPTIONS "-Wno-terminate")

if (APPLE)
	# Because of new linker implementation in XCode 15 https://developer.apple.com/forums/thread/737707
	add_link_options("-ld_classic")
endif()

enable_testing()

add_subdirectory(RVM)
add_subdirectory(tests)