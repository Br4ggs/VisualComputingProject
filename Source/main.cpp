#include "header/main.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//imgui related includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include "header/shaderProgram.h"
#include "header/texturedScreenQuad.h"
#include "header/rayMarcher.h"

int main()
{
    std::cout << "Hello " << testmacro << std::endl;

    //glm test code; wowee a matrix inverse!
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 inverse = glm::inverse(matrix);

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 800, "VisualComputingProject", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    int version = gladLoadGL();
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1000, 800);

    ShaderProgram shaderProg;
    shaderProg.attachVertexShader("..\\..\\..\\vertexScreenQuad.glsl"); //since the executable is located in Source/out/build/x64-Debug
    shaderProg.attachFragmentShader("..\\..\\..\\fragmentScreenQuad.glsl");

    shaderProg.compile();

    TexturedScreenQuad screenquad(&shaderProg);

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    RayMarcher marcher(1000, 800);

    marcher.render();
    unsigned char* dataptr = marcher.getRenderData();
    screenquad.writeToTexture(1000, 800, dataptr);

    while (!glfwWindowShouldClose(window))
    {
        //imgui stuff
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();
        //ImGui::ShowDemoWindow();

        //draws triangle
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenquad.render();

        //glBindVertexArray(vertexArray);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //render
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //delete[] testTex;

    screenquad.destroy();
    shaderProg.destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
