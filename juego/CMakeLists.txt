cmake_minimum_required(VERSION 3.29)
project(juego)

set(CMAKE_CXX_STANDARD 20)

add_executable(juego src/main.cpp src/board.hpp src/board.cpp src/TTEntry.h)
