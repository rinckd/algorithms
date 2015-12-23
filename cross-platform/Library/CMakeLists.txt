cmake_minimum_required(VERSION 2.8)


add_definitions(-std=c++11)
project(Library)

set(HEADER_FILES
    Algorithms.h
    Expected.h
    MoveClassExample.h
    )

set(SOURCE_FILES
    Algorithms.cpp
    MoveClassExample.cpp
    )


add_library(Library STATIC ${SOURCE_FILES} ${HEADER_FILES})
