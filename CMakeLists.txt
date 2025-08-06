cmake_minimum_required(VERSION 3.10)
project(SimText VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for better optimization and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

# Main executable
add_executable(simtext 
    src/main.cpp
    src/text_processor.cpp
    src/similarity_calculator.cpp
    src/shingling.cpp
)

target_include_directories(simtext PRIVATE include)

# Test executable
add_executable(test_simtext 
    tests/test_similarity.cpp
    src/text_processor.cpp
    src/similarity_calculator.cpp
    src/shingling.cpp
)

target_include_directories(test_simtext PRIVATE include)

# Enable testing
enable_testing()
add_test(NAME unit_tests COMMAND test_simtext)