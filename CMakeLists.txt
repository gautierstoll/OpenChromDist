cmake_minimum_required(VERSION 3.31)
project(OpenChromDist)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenChromDist main.cpp
        PeakBasedDist.cpp
        PeakBasedDist.h)
