cmake_minimum_required(VERSION 3.5)

project("TLD_Redux"
    VERSION 0.0
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(WIN32)
    message(STATUS "using 32bit build for TLD")
    add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH -DWIN32 -D_WIN32 -DSYSTEM_NAME="Windows -DRAKSAMP_CLIENT")

    set(USE_MSVC_RUNTIME_LIBRARY_DLL false)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _MBCS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _MBCS")

    set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
    )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else ()
    message(FATAL_ERROR "TLD CAN ONLY BE BUILT UNDER WIN32")
endif ()

include(FetchContent)
FetchContent_Declare(
    ktsignal
    GIT_REPOSITORY https://github.com/kin4stat/ktsignal.git
    GIT_TAG f08a19d9a00ad5284871bf44178d835da7b26ebf
)
FetchContent_Declare(
    kthook
    GIT_REPOSITORY https://github.com/kin4stat/kthook.git
    GIT_TAG 21ca70481273d1efcc98ca49063178e46277009b
)
FetchContent_MakeAvailable(ktsignal kthook)

file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_subdirectory(lib)
add_subdirectory(test)
find_package(PluginSDK REQUIRED gtasa)

add_library(${PROJECT_NAME} SHARED)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS off
)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".asi"
)

if(NOT "${DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC ${DXSDK_DIR}/Lib/x86)
elseif(NOT "$ENV{DXSDK_DIR}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Include)
    target_link_directories(${PROJECT_NAME} PUBLIC $ENV{DXSDK_DIR}/Lib/x86)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib/httplib")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib/cryptopp/include")
target_link_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib/cryptopp/lib")
target_link_libraries(${PROJECT_NAME} PUBLIC
        sampapi
        kthook
        RakNet
        rakhook
        nlohmann_json
        d3d9
        d3dx9
        glm
        imgui
        PluginSDK::gtasa
        cryptlib
)

# the post-build command to copy the .dll file only
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
    "D:/Game/GTA San Andreas/$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}>"
)
