cmake_minimum_required(VERSION 3.15)
project(sprint_2)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set(TOPIC_1 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_1")
set(TOPIC_2 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_2")


include_directories("${TOPIC_1}")
include_directories("${TOPIC_2}")

#----------------------------------------------------
add_executable( sprint_2 sprint_2.cpp)


