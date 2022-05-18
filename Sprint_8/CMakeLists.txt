cmake_minimum_required(VERSION 3.15)
project(SPRINT_8)
set(CMAKE_CXX_STANDARD 20)

#----------------------------------------------------
set(SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/document.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/process_queries.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/read_input_functions.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/remove_duplicates.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/request_queue.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/search_server.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/string_processing.cpp
${CMAKE_CURRENT_SOURCE_DIR}/Topic_1/search_system/test_example_functions.cpp
)
add_executable( sprint_8 sprint_8.cpp ${SOURCES})

