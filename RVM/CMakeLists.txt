set(RVM_SOURCES main.cpp VM.cpp tokenizer.cpp scanner.cpp parser.cpp logging.hpp)

add_executable(RVM ${RVM_SOURCES})
target_compile_options(RVM PRIVATE ${RVM_COMPILE_OPTIONS})
add_library(RVM_LIB STATIC ${RVM_SOURCES})