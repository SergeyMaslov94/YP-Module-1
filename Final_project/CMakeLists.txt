cmake_minimum_required(VERSION 3.15)
project(FINAL_PROJECT)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set (SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/document.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/read_input_functions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/request_queue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/search_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/string_processing.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/unit_tests.cpp)
#----------------------------------------------------
add_executable( final_project ${SOURCES} main.cpp)

