cmake_minimum_required(VERSION 3.14)
project(LimitOrderBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header files directory
include_directories(include)

# Find Python package directly
find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)

# Fetch pybind11 path automatically from Python environment
execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_package(pybind11 REQUIRED)

# Source files for C++ Benchmark Executable
set(SOURCES
    src/OrderBook.cpp
    src/Server.cpp
    src/main.cpp
)

# Executable output
add_executable(LimitOrderBook ${SOURCES})

# Link Winsock library for TCP Server (Windows)
target_link_libraries(LimitOrderBook PRIVATE ws2_32)

# Python Bindings Module
pybind11_add_module(trading_engine 
    src/bindings.cpp 
    src/OrderBook.cpp
)