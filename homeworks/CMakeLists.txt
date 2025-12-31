cmake_minimum_required(VERSION 3.24)
project(CPP_Homeworks)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using CXX compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX standard: ${CMAKE_CXX_STANDARD}")

add_subdirectory(h01)
add_subdirectory(h02)
add_subdirectory(h03)
add_subdirectory(h04)
add_subdirectory(h05)
add_subdirectory(slovnik_extended)

