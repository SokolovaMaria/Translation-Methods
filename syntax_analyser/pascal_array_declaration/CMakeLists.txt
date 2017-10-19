cmake_minimum_required(VERSION 3.8)
project(pascal_array_declaration)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-O2 -Wall -Wextra -Werror")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

set(SOURCE_FILES token.h lex_analyser.h lex_analyser.cpp parser.h parser.cpp )

add_library(pascal_array_declaration ${SOURCE_FILES})



