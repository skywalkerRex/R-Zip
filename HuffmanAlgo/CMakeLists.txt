cmake_minimum_required(VERSION 3.10)
project(HuffmanAlgo)

# Add library for HuffmanAlgo
add_library(HuffmanAlgo src/HuffmanTree.cpp src/HuffmanCoder.cpp)

# Include directories
target_include_directories(HuffmanAlgo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
