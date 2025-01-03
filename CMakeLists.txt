cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(rzip LANGUAGES CXX CUDA)

option(COMPRASS_DEBUG_BUILD "Is in Compress Debug Build" FALSE)
option(DECOMPRASS_DEBUG_BUILD "Is in Decompress Debug Build" FALSE)
option(DEBUG_BUILD "Is in Debug Build" FALSE)
option(RELEASE_BUILD "Is in Release Build" FALSE)

# Add subdirectory for HuffmanAlgo
add_subdirectory(HuffmanAlgo)

add_executable(rzip rzip.cpp)

# Link HuffmanAlgo library
target_link_libraries(rzip HuffmanAlgo)

if(COMPRASS_DEBUG_BUILD)
    add_compile_definitions(IS_COMPRASS_DEBUG_BUILD=1)
else()
    add_compile_definitions(IS_COMPRASS_DEBUG_BUILD=0)
endif()

if(DECOMPRASS_DEBUG_BUILD)
    add_compile_definitions(IS_DECOMPRASS_DEBUG_BUILD=1)
else()
    add_compile_definitions(IS_DECOMPRASS_DEBUG_BUILD=0)
endif()

if(DEBUG_BUILD)
    add_compile_definitions(IS_DEBUG_BUILD=1)
else()
    add_compile_definitions(IS_DEBUG_BUILD=0)
endif()

if(RELEASE_BUILD)
    add_compile_definitions(IS_RELEASE_BUILD=1)
else()
    add_compile_definitions(IS_RELEASE_BUILD=0)
endif()

target_compile_features(rzip PUBLIC cxx_std_17)

set_target_properties(rzip PROPERTIES CUDA_SEPARABLE_COMPILATION ON)