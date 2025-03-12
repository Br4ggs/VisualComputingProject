//
// Created by Kishan on 06/03/2025.
//

#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <GLFW/glfw3.h>

// Initializes Dear ImGui for a given GLFW window.
void ImGui_Init(GLFWwindow* window);

// Call at the beginning of each frame to set up a new ImGui frame.
void ImGui_NewFrame();

// Call after your rendering code to render the ImGui draw data.
void ImGui_Render();

// Cleanup ImGui resources.
void ImGui_Shutdown();

#endif // IMGUI_LAYER_H
