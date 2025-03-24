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

    bool dirty = false;

    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Rendering"))
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
            marchingCubes->drawUI(dirty);
            break;
        default:
            break;
        }
    }

    //call to Scene ui
    scene->drawUI(dirty);

    if (selectedRenderBackend == 1 && dirty)
    {
        marchingCubes->regenerateMarchingCubes(); //TODO: trigger only when scene has actually changed
    }

    if (selectedRenderBackend == 0 && ImGui::Button("Render"))
    {
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

    glfwWindowHint(GLFW_DEPTH_BITS, 24); // at least 24 bits
    GLFWwindow* window = glfwCreateWindow(1000, 800, "VisualComputingProject", nullptr, nullptr);

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
    shaderProgMarcher.attachVertexShader("vertexScreenQuad.glsl"); //since the executable is located in Source/out/build/x64-Debug
    shaderProgMarcher.attachFragmentShader("fragmentScreenQuad.glsl");
    shaderProgMarcher.compile();

    screen = new TexturedScreenQuad(&shaderProgMarcher);

    ShaderProgram shaderProgMarchingCubes;
    shaderProgMarchingCubes.attachVertexShader("vertexMarchingCubes.glsl");
    shaderProgMarchingCubes.attachFragmentShader("fragmentMarchingCubes.glsl");
    shaderProgMarchingCubes.compile();

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    scene = new Scene();

    marcher = new RayMarcher(1000, 800);
    marchingCubes = new MarchingCubes(1000, 800, scene, &shaderProgMarchingCubes);
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        imGuiTest();

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch (selectedRenderBackend)
        {
        case 0: //cpu-based sphere tracing. just render the screen quad
            screen->render();
            break;
        case 1: //marching cubes.
            marchingCubes->render();
            break;
        }

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
