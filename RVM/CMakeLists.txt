set(RVM_SOURCES main.cpp VM.cpp tokenizer.cpp scanner.cpp parser.cpp)
add_executable(RVM ${RVM_SOURCES})
target_compile_options(RVM PRIVATE ${RVM_COMPILE_OPTIONS})