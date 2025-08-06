cmake_minimum_required(VERSION 3.10)
project(SimText VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simtext 
    src/main.cpp
    src/text_processor.cpp
    src/similarity_calculator.cpp
)

target_include_directories(simtext PRIVATE include)