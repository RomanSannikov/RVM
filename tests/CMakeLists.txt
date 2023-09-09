set(TEST_SOURCES test.cpp testFunctions.hpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip #1.14.0v
)
FetchContent_MakeAvailable(googletest)

add_executable(unittests ${TEST_SOURCES})
target_link_libraries(unittests PUBLIC gtest_main RVM_LIB)
target_compile_definitions(unittests PRIVATE TESTING_DATA_PATH_PREFIX="${CMAKE_CURRENT_SOURCE_DIR}/data/")

add_test(NAME unittests COMMAND unittests)