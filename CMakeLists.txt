cmake_minimum_required(VERSION 3.14)
project(LimitOrderBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include header files directory
include_directories(include)

#source files
set(SOURCES
    src/OrderBook.cpp
    src/main.cpp
)

#Execlutable output
add_executable(LimitOrderBook ${SOURCES})