cmake_minimum_required(VERSION 3.22)
project(LogLib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(WITH_TESTS "build with tests" ON)
option(WITH_LITERALS "build with log user-defined literals" ON)
if (WITH_LITERALS)
add_definitions(-DLOG_LITERALS)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(WITH_TESTS)
    add_subdirectory(tests)
endif()