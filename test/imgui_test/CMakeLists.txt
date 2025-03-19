cmake_minimum_required(VERSION 3.15)

project("CEF_TEST"
    VERSION 0.0
)

# Find all source files in the project directory
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.hpp"
)
file(GLOB_RECURSE DEP_FILES
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/backends/imgui_impl_dx9.cpp"
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/backends/imgui_impl_win32.cpp"
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/imgui_widgets.cpp"
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/../../lib/imgui/imgui_tables.cpp"
)

include_directories("${PROJECT_SOURCE_DIR}/../../lib/imgui/backends")
include_directories("${PROJECT_SOURCE_DIR}/../../lib/imgui/")

list(APPEND SRC_FILES ${DEP_FILES})

# Add the executable for the project
add_executable(${PROJECT_NAME} ${SRC_FILES})


if(NOT "${DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Lib/x86)
elseif(NOT "$ENV{DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Lib/x86)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC d3d9 d3dx9)

# Set C++ standard to C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
