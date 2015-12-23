cmake_minimum_required(VERSION 2.8)
project(BatsCodingExercise)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(bats_coding_exercise ${SOURCE_FILES})

include_directories(Library)

add_subdirectory(Library)
add_subdirectory(GoogleTests)

target_link_libraries(bats_coding_exercise Library)
set(CMAKE_CXX_FLAGS "-g -Wall")
