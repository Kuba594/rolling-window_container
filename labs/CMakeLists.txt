cmake_minimum_required(VERSION 3.24)
project(CPP_Labs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using CXX compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX standard: ${CMAKE_CXX_STANDARD}")

add_subdirectory(c01-nasobilka)
add_subdirectory(c02-textparser)
add_subdirectory(c03-frequencies)
