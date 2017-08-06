project(cbcmaker CXX C)
cmake_minimum_required(VERSION 3.1)

file(GLOB SOURCES cbcmaker.cpp)

add_executable(cbcmaker ${SOURCES})