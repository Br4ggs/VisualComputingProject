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
#include "header/marchingCubes.h"

int selectedRenderBackend = 0;

TexturedScreenQuad* screen;
RayMarcher* marcher;
MarchingCubes* marchingCubes;
Scene* scene;

void imGuiTest()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(400, 680));

    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("RenderingEE"))
    {
        //rendering backend dropdown
        const char* renderTypes[] = { "sphere tracing (cpu)", "marching cubes"};
        const char* renderPreview = renderTypes[selectedRenderBackend];

        if (ImGui::BeginCombo("rendering backend", renderPreview))
        {
            for (int n = 0; n < 2; n++)
            {
                const bool selected = (selectedRenderBackend == n);
                if (ImGui::Selectable(renderTypes[n], selected))
                {
                    std::cout << "i was clicked" << std::endl;
                    selectedRenderBackend = n;
                }

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        //render settings
        switch (selectedRenderBackend)
        {
        case 0: //sphere tracing
            marcher->drawUI();
            break;
        case 1: //marching cubes
            marchingCubes->drawUI();
            break;
        default:
            break;
        }
    }

    //call to Scene ui
    scene->drawUI();

    if (selectedRenderBackend == 0 && ImGui::Button("Render"))
    {
        std::cout << "wuz clicked" << std::endl;
        marcher->render(scene);
        unsigned char* dataptr = marcher->getRenderData();
        screen->writeToTexture(1000, 800, dataptr);
    }

    ImGui::End();
}

int main()
{
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

    ShaderProgram shaderProgMarcher;
    shaderProgMarcher.attachVertexShader("..\\..\\..\\vertexScreenQuad.glsl"); //since the executable is located in Source/out/build/x64-Debug
    shaderProgMarcher.attachFragmentShader("..\\..\\..\\fragmentScreenQuad.glsl");
    shaderProgMarcher.compile();

    screen = new TexturedScreenQuad(&shaderProgMarcher);

    ShaderProgram shaderProgMarchingCubes;
    shaderProgMarchingCubes.attachVertexShader("..\\..\\..\\vertexMarchingCubes.glsl");
    shaderProgMarchingCubes.attachFragmentShader("..\\..\\..\\fragmentMarchingCubes.glsl");
    shaderProgMarchingCubes.compile();

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    marcher = new RayMarcher(1000, 800);
    marchingCubes = new MarchingCubes();

    scene = new Scene();

    while (!glfwWindowShouldClose(window))
    {
        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        imGuiTest();

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screen->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //render
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    screen->destroy();

    shaderProgMarcher.destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete screen;
    delete marcher;
    delete scene;

    return 0;
}
