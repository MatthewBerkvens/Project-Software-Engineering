cmake_minimum_required(VERSION 3.6)
project(Project_Software_Engineering)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES src/main_release.cpp)

# Create RELEASE target
add_executable(Project_Software_Engineering ${RELEASE_SOURCE_FILES})

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES src/main_debug.cpp)

# Create DEBUG target
add_executable(Project_Software_Engineering_debug ${DEBUG_SOURCE_FILES})