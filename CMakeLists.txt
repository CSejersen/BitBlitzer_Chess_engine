cmake_minimum_required(VERSION 3.26)
project(BBchess_engine)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_MODULE_PATH "/Applications/CLion.app/Contents/bin/cmake/mac/share/cmake-3.26/Modules")
set(SDL2_PATH "/opt/homebrew/Cellar/sdl2/2.28.5")
set(SDL2_INCLUDE_DIR ${SDL2_PATH}/include)
set(SDL2_LIB_DIR ${SDL2_PATH}/lib)

find_package(SDL2 REQUIRED PATHS ${SDL2_PATH})

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(BBchess_engine main.cpp
        bitboards.h
        bitboards.cpp
        move.h
        move.cpp
        magics.h
        SDL_functions.cpp
        SDL_functions.h
)

target_link_libraries(BBchess_engine ${SDL2_LIBRARY} )