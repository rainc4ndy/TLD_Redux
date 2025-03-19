cmake_minimum_required(VERSION 3.20)
project(RakNet LANGUAGES CXX)

# Set C++14 as the standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files and headers
file(GLOB_RECURSE SRC_FILES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp"
)

# Create a static library for RakNet
add_library(RakNet STATIC ${SRC_FILES})

# Include directories
target_include_directories(RakNet PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # Include main directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src      # Include source directory
)

# Preprocessor definitions, adjust depending on platform or project needs
target_compile_definitions(RakNet PRIVATE
        WIN32
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
        _WINSOCK_DEPRECATED_NO_WARNINGS  # Suppress deprecated warnings
)

# Link necessary libraries for network functionality (if building on Windows)
if (WIN32)
    target_link_libraries(RakNet PRIVATE ws2_32 comctl32)

    # Add MSVC specific compiler flags
    target_compile_options(RakNet PRIVATE
            /permissive
            # Add additional MSVC flags here if needed
    )
endif()

