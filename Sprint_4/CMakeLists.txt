cmake_minimum_required(VERSION 3.15)
project(SPRINT_4)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set(TOPIC_1 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_1")
set(TOPIC_2 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_2")
set(TOPIC_3 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_3")
set(TOPIC_4 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_4")

include_directories("${TOPIC_1}")
include_directories("${TOPIC_2}")
include_directories("${TOPIC_3}")
include_directories("${TOPIC_4}")
#----------------------------------------------------
add_executable( sprint_4 sprint_4.cpp)

