cmake_minimum_required(VERSION 3.24)
project(project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using CXX compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX standard: ${CMAKE_CXX_STANDARD}")

add_executable(project src/main.cpp
        src/rolling_stats.h
        src/matrix.h
        src/Incremental_stats.h
        src/data_loader.h
        src/data_loader.cpp)
add_executable(tests tests/testy.cpp
)
add_executable(tests_stats tests/tests_stats/testy_stats.cpp
)
add_executable(tests_incrementa_stats tests/tests_incrementa_stats/tests_incremental_stats.cpp
)
