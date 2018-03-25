cmake_minimum_required(VERSION 3.6)
project(Project_Software_Engineering)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Add Subdirectories of source files
file(GLOB SRC_LIBRARIES src/lib/*)
file(GLOB SRC_TINYXML src/lib/tinyxml/*)
file(GLOB SRC_CLASSES src/classes/*)
file(GLOB SRC_PARSER src/parser/*)
file(GLOB SRC_UTILS src/utils/*)
file(GLOB SRC_TESTS src/tests/*)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        src/main_release.cpp
        ${SRC_LIBRARIES}
        ${SRC_TINYXML}
        ${SRC_CLASSES}
        ${SRC_PARSER}
        ${SRC_UTILS})

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        src/main_debug.cpp
        ${SRC_LIBRARIES}
        ${SRC_TINYXML}
        ${SRC_CLASSES}
        ${SRC_PARSER}
        ${SRC_UTILS}
        ${SRC_TESTS})

# Create RELEASE target
add_executable(PSE_Release ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(PSE_Debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(PSE_Debug gtest)