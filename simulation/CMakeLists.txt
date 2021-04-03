cmake_minimum_required(VERSION 3.10)
project(Simulation)


set(CMAKE_CPP_STANDARD 11)
set(CMAKE_CPP_STANDARD_REQUIRED True)


add_subdirectory(src)
add_subdirectory(test)



if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
    "No in source builds allowed. Create a separate build directory.
    SOURCE_DIR=${CMAKE_SOURCE_DIR}  BINARY_DIR=${CMAKE_BINARY_DIR} ")
endif()