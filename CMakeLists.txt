cmake_minimum_required(VERSION 3.23)
project(A2_initial)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(__MACOSX)

add_executable(main1
        main.cpp
        knight2.cpp)
