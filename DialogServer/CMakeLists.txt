cmake_minimum_required(VERSION 3.21)
project(DialogServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(DialogServer main.cpp DialogServer.cpp DialogServer.h)
