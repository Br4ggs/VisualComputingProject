cmake_minimum_required(VERSION 3.10)

project(VisualComputingProject)

add_executable(VisualComputingProject main.cpp glad.c)

set(LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

target_include_directories(VisualComputingProject PRIVATE ${LIBRARY_PATH}/include)

if (APPLE)
  message(STATUS "Configuring for macOS")
  target_link_libraries(VisualComputingProject 
    ${LIBRARY_PATH}/lib/libglfw.3.4.dylib
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
elseif (WIN32)
  message(STATUS "Configuring for Windows")
  target_link_libraries(VisualComputingProject PRIVATE ${LIBRARY_PATH}/lib/glfw3.lib)
endif()

