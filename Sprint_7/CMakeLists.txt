cmake_minimum_required(VERSION 3.15)
project(SPRINT_7)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set(TOPIC_1 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_1")

#----------------------------------------------------
add_executable( sprint_7 sprint_7.cpp   "Topic_2/lesson_3_string_view/log_duration.hpp")

