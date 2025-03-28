cmake_minimum_required(VERSION 3.10)

project(VisualComputingProject)

add_executable(VisualComputingProject
    main.cpp
    shaderProgram.cpp
    texturedScreenQuad.cpp
    rayMarcher.cpp
    marchingCubes.cpp
    linearizeScene.cpp
    scene.cpp
    sdfSphere.cpp
    sdfBox.cpp
    sdfCylinder.cpp
    sdfPlane.cpp
    opUnion.cpp
    opSmoothUnion.cpp
    opIntersect.cpp
    opDifference.cpp
    opModulo.cpp
    worldObject.cpp
    addOperatorModal.cpp
    addPrimitiveModal.cpp
    inputController.cpp
    IRenderStrategy.cpp
    OpenGLStrategy.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/glad.c
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/src/stb_image.cpp)

set(LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

set_property(
    TARGET VisualComputingProject
    PROPERTY CXX_STANDARD 17
)

target_include_directories(VisualComputingProject PRIVATE ${LIBRARY_PATH}/include)
target_include_directories(VisualComputingProject PRIVATE ${PROJECT_SOURCE_DIR}/header)

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

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vertexScreenQuad.glsl" "${CMAKE_BINARY_DIR}/vertexScreenQuad.glsl" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/fragmentScreenQuad.glsl" "${CMAKE_BINARY_DIR}/fragmentScreenQuad.glsl" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vertexMarchingCubes.glsl" "${CMAKE_BINARY_DIR}/vertexMarchingCubes.glsl" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/fragmentMarchingCubes.glsl" "${CMAKE_BINARY_DIR}/fragmentMarchingCubes.glsl" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/oglMarcher.vert" "${CMAKE_BINARY_DIR}/oglMarcher.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/oglMarcher.frag" "${CMAKE_BINARY_DIR}/oglMarcher.frag" COPYONLY)

