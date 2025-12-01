cmake_minimum_required(VERSION 3.10)
project(BracketChecker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MONGOOSE_DIR ${CMAKE_SOURCE_DIR}/lib/mongoose)

add_executable(BracketChecker src/main.cpp ${MONGOOSE_DIR}/mongoose.c include/CodePairsCheck.h)

target_include_directories(BracketChecker PRIVATE include ${MONGOOSE_DIR})

target_link_libraries(BracketChecker PRIVATE ws2_32)
