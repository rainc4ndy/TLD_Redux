message(STATUS "Adding libs")

add_subdirectory(json)
add_subdirectory(SAMP-API)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glm")

add_subdirectory(RakNet)
target_include_directories(RakNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/RakNet/include")

add_subdirectory(rakhook)
target_include_directories(rakhook INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/rakhook/include")

# ImGui
add_library(imgui STATIC
        imgui/backends/imgui_impl_dx9.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_stdlib.cpp
)
target_include_directories(imgui PUBLIC
        imgui
        imgui/backends
)

