cmake_minimum_required(VERSION 3.10)
project(BracketChecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BracketChecker src/main.cpp include/CodePairsCheck.h)

target_include_directories(BracketChecker PRIVATE include)
