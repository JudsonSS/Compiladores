cmake_minimum_required(VERSION 3.0.0)
project(Tradutor)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES ast.cpp gen.cpp checker.cpp lexer.cpp parser.cpp symtable.cpp error.cpp tradutor.cpp)
add_executable(tradutor ${SOURCE_FILES})