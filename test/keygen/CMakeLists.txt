cmake_minimum_required(VERSION 3.15)

project("keygen"
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
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

target_include_directories(${PROJECT_NAME} PRIVATE "D:/TLD_Redux/lib/cryptopp/include")
target_link_directories(${PROJECT_NAME} PRIVATE "D:/TLD_Redux/lib/cryptopp/lib")
target_link_libraries(${PROJECT_NAME} cryptlib)