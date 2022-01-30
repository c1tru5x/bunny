cmake_minimum_required(VERSION 3.17)
project(bunny)
set(CMAKE_CXX_STANDARD 17)

include_directories(bunny)

add_executable(bunny
        bunny/connector.h
        bunny/main.cpp
        bunny/OffsetParser.h bunny/sound.cpp bunny/sound.h)

# dont forget to use vs as compiler !!!!