cmake_minimum_required(VERSION 3.15)
project(SPRINT_3)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set(TOPIC_1 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_1")
#set(TOPIC_2 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_2")

include_directories("${TOPIC_1}")
#include_directories("${TOPIC_2}")
#----------------------------------------------------
add_executable( sprint_3 sprint_3.cpp  "Topic_1/lesson_3.hpp" "Topic_1/lesson_4.hpp" "Topic_1/lesson_6.hpp" "Topic_1/theory_7.hpp" "Topic_2/theory_1.hpp" "Topic_2/lesson_1.hpp")

