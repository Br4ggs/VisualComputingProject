cmake_minimum_required(VERSION 3.10)

project(VisualComputingProject)

add_executable(VisualComputingProject main.cpp glad.c)
target_include_directories(VisualComputingProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/include)
target_link_libraries(VisualComputingProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lib/glfw3.lib)
target_link_libraries(VisualComputingProject PRIVATE opengl32.lib)