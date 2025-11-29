cmake_minimum_required(VERSION 3.10)
project(BracketChecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES
        src/core/BracketChecker.cpp
        src/core/Stack.cpp
)

set(ERROR_SOURCES
        src/error/ErrorHandler.cpp
)

set(NETWORK_SOURCES
        src/network/HTTPServer.cpp
        src/network/APIHandler.cpp
        src/network/RequestHandler.cpp
)

set(SERIALIZATION_SOURCES
        src/serialization/JSONSerializer.cpp
)

set(SOURCES
        src/main.cpp
        ${CORE_SOURCES}
        ${ERROR_SOURCES}
        ${NETWORK_SOURCES}
        ${SERIALIZATION_SOURCES}
)

add_executable(BracketChecker ${SOURCES})

if(WIN32)
    target_link_libraries(BracketChecker ws2_32)
endif()

target_include_directories(BracketChecker PRIVATE include)

if(MSVC)
    target_compile_options(BracketChecker PRIVATE /W4)
else()
    target_compile_options(BracketChecker PRIVATE -Wall -Wextra)
endif()