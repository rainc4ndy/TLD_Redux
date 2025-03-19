cmake_minimum_required(VERSION 3.15)

project("BinaryPacker"
    VERSION 1.0
)

# Find all source files in the project directory
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.hpp"
)

list(APPEND SRC_FILES ${DEP_FILES})
# Add the executable for the project
add_executable(${PROJECT_NAME} ${SRC_FILES})
# Set C++ standard to C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
