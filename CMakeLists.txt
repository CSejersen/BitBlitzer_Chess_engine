cmake_minimum_required(VERSION 3.26)
project(BBchess_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(BBchess_engine main.cpp
        bitboards.h
        bitboards.cpp
        move.h
        move.cpp
        magics.h
)
