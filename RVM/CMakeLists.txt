file(GLOB RVM_SOURCES main.cpp VM.cpp EpsilonGC.cpp STWGC.cpp tokenizer.cpp scanner.cpp parser.cpp logging.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_executable(RVM ${RVM_SOURCES})
target_compile_options(RVM PRIVATE ${RVM_COMPILE_OPTIONS})
add_library(RVM_LIB STATIC ${RVM_SOURCES})