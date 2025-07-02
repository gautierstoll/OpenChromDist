cmake_minimum_required(VERSION 3.28)
project(OpenChromDist)

set(CMAKE_CXX_STANDARD 20)

# add_executable(OpenChromDist main.cpp
#        PeakBasedDist.cpp
#        PeakBasedDist.h
 #       SinglePeakDist.cpp
 #       SinglePeakDist.h
 #       BarcodePeakDist.cpp
 #       BarcodePeakDist.h)

add_executable(OpenChromDistTest maintTest.cpp
        PeakBasedDist.cpp
        PeakBasedDist.h)
