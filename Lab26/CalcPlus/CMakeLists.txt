cmake_minimum_required(VERSION 3.0.0)
project(CalcPlus)

# Configuração do C++
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

# Configuração do Flex
find_package(FLEX)
FLEX_TARGET(Lexer calc.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

# Configuração do Bison
find_package(BISON)
BISON_TARGET(Parser calc.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp 
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

# Executável depende da saída do Flex e do Bison
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(calc ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
