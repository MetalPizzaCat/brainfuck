cmake_minimum_required(VERSION 3.0.0)
project(brainfuck VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()
set(CMAKE_CXX_STANDARD 17)

add_executable(brainfuck main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
