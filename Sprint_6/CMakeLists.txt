cmake_minimum_required(VERSION 3.15)
project(SPRINT_6)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------
set(TOPIC_1 "${CMAKE_CURRENT_SOURCE_DIR}/Topic_1")

#----------------------------------------------------
add_executable( sprint_6 sprint_6.cpp "Topic_1/theory_10.hpp" "Topic_1/lesson_10_2.hpp" "Topic_2/lesson_1.hpp" "Topic_2/tests.hpp" "Topic_2/SingleLinkedList.hpp" "Topic_3/lesson_1.hpp")

