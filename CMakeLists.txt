cmake_minimum_required(VERSION 3.28)
project(OpenChromDist
        VERSION 0.10.1
        LANGUAGES CXX)

message(STATUS "Project version: ${PROJECT_VERSION}")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in   # template dans le dossier source
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h      # generated file by build command
        @ONLY
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

 add_executable(OpenChromDist main.cpp
         PeakBasedDist.cpp
         PeakDistanceMatrix.cpp
         CellData.h
         CellInOutMeanVar.h
         CellDataMeanVar.h
         CellData.h
         CellDataMeanVar.h
         CellInOutMeanVar.h
 )
target_link_libraries(OpenChromDist PRIVATE cmph)
target_compile_options(OpenChromDist PRIVATE $<$<CONFIG:Release>:-O3 -march=native -flto -funroll-loops>)

target_compile_options(OpenChromDist PRIVATE $<$<CONFIG:Debug>:-O0 -g -Wall>)

#add_executable(OpenChromDistTest mainTest.cpp
#        PeakBasedDist.cpp
#        PeakBasedDist.h
#        PeakDistanceMatrix.cpp
#        PeakDistanceMatrix.h)
