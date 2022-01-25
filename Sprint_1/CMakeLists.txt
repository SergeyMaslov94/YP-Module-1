cmake_minimum_required(VERSION 3.15)
project(SPRINT_1)
set(CMAKE_CXX_STANDARD 17)

#----------------------------------------------------

# тут будут все пути к либам и заголовкам
set(1_BASIC_ALGORITHMS "${CMAKE_CURRENT_SOURCE_DIR}/1. Basic algorithms")
set(3_LAMBDA_FUNCTIONS "${CMAKE_CURRENT_SOURCE_DIR}/3. Lambda function")
set(4_NUMERIC_TYPES "${CMAKE_CURRENT_SOURCE_DIR}/4. Numeric types")
set(5_PAIR_AND_TUPLE "${CMAKE_CURRENT_SOURCE_DIR}/5. Pair and Tuple")
set(6_FUNCTION_TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/6. Function templates")

# Тут я их объявлю
include_directories("${1_BASIC_ALGORITHMS}")
include_directories("${3_LAMBDA_FUNCTIONS}")
include_directories("${4_NUMERIC_TYPES}")
include_directories("${5_PAIR_AND_TUPLE}")
include_directories("${6_FUNCTION_TEMPLATES}")

#----------------------------------------------------
add_executable( simple_runner main.cpp)

