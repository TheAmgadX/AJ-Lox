cmake_minimum_required(VERSION 3.13)
project(AJ_LOX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# .cc files here.
set (
    COMMON_SOURCES
    src/aj_lox.cc
    src/scanner.cc
)

add_executable(lex
    src/main.cc
    ${COMMON_SOURCES}
)

target_include_directories(lex PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
