cmake_minimum_required(VERSION 2.8)
project(MAR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(cws/include cws/3rdparty/)


set(SOURCE_FILES main.cpp Mare.cpp cws/src/JiebaSegment.cpp)
add_executable(MAR ${SOURCE_FILES})
