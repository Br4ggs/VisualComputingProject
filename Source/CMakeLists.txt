cmake_minimum_required(VERSION 3.10)

project(VisualComputingProject)

add_executable(VisualComputingProject
    main.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/glad.c
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_widgets.cpp
        MarchingCubes.cpp
        header/MarchingCubes.h)

set(LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

set_property(
    TARGET VisualComputingProject
    PROPERTY CXX_STANDARD 11
)

target_include_directories(VisualComputingProject PRIVATE ${LIBRARY_PATH}/include)

if (APPLE)
    message(STATUS "Configuring for macOS")
    find_package(glfw3 REQUIRED)
    target_link_libraries(VisualComputingProject 
        glfw
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif (WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(VisualComputingProject PRIVATE ${LIBRARY_PATH}/lib/glfw3.lib)
endif()
