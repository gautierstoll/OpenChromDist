cmake_minimum_required(VERSION 3.28)
project(OpenChromDist
        VERSION 0.9.1
        LANGUAGES CXX)

message(STATUS "Project version: ${PROJECT_VERSION}")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in   # template dans le dossier source
        ${CMAKE_CURRENT_BINARY_DIR}/version.h      # fichier généré dans le build
        @ONLY
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

 add_executable(OpenChromDist main.cpp
         PeakBasedDist.cpp
         PeakBasedDist.h
         PeakDistanceMatrix.cpp
         PeakDistanceMatrix.h
 )

#add_executable(OpenChromDistTest mainTest.cpp
#        PeakBasedDist.cpp
#        PeakBasedDist.h
#        PeakDistanceMatrix.cpp
#        PeakDistanceMatrix.h)
