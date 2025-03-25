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
#include "header/OpenGLMarcher.h"
#include "header/types.h"

#define STRINGIFY(x) #x // makes a string of x
#define TOSTRING(x) STRINGIFY(x) // expands x

RenderType::Type selectedRenderBackend = RenderType::SPHERE_MARCHING_GPU;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

TexturedScreenQuad* screen;
RayMarcher* marcher;
MarchingCubes* marchingCubes;
OpenGLMarcher* oglMarcher;
Scene* scene;

void imGuiTest()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(400, 680));

    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Rendering"))
    {
        const char** renderTypeNames;
        RenderType::allNames(&renderTypeNames);
        const char** renderTypes = renderTypeNames;
        const char* renderPreview = RenderType::getName(selectedRenderBackend);

        if (ImGui::BeginCombo("rendering backend", renderPreview))
        {
            for (int n = 0; n < (int)RenderType::COUNT; n++)
            {
                const bool selected = (selectedRenderBackend == n);
                if (ImGui::Selectable(renderTypes[n], selected))
                {
                    std::cout << "i was clicked: " << renderTypes[n] << std::endl;
                    selectedRenderBackend = RenderType::getType(n);
                }

                if (selected) ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        switch (selectedRenderBackend)
        {
            case RenderType::SPHERE_MARCHING_CPU:
                marcher->drawUI();
                break;
            case RenderType::CUBE_MARCHING_CPU:
                marchingCubes->drawUI();
                break;
            case RenderType::SPHERE_MARCHING_GPU:
                oglMarcher->drawUI();
                break;
            default:
                throw std::logic_error("selected render backend does not exist, can not draw ui");
                break;
        }
    }

    //call to Scene ui
    scene->drawUI();

    switch (selectedRenderBackend)
    {
        case RenderType::SPHERE_MARCHING_CPU:
            marcher->drawUI();
            if (ImGui::Button("Render")) {
                marcher->render(scene);
                unsigned char* dataptr = marcher->getRenderData();
                screen->writeToTexture(1000, 800, dataptr);
            }
            break;
        case RenderType::CUBE_MARCHING_CPU:
            marchingCubes->regenerateMarchingCubes();
            break;
        case RenderType::SPHERE_MARCHING_GPU:
            if (ImGui::Button("Render")) {
                oglMarcher->dirty = true;
                oglMarcher->render();
            }
            break;
        default:
            throw std::logic_error("selected render backend does not exist, can not render scene");
            break;
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
    shaderProgMarcher.attachVertexShader(TOSTRING(SHADER_PATH) "vertexScreenQuad.vert");
    shaderProgMarcher.attachFragmentShader(TOSTRING(SHADER_PATH) "fragmentScreenQuad.frag");
    shaderProgMarcher.compile();

    screen = new TexturedScreenQuad(&shaderProgMarcher);

    ShaderProgram shaderProgMarchingCubes;
    shaderProgMarchingCubes.attachVertexShader(TOSTRING(SHADER_PATH) "vertexMarchingCubes.vert");
    shaderProgMarchingCubes.attachFragmentShader(TOSTRING(SHADER_PATH) "fragmentMarchingCubes.frag");
    shaderProgMarchingCubes.compile();

    ShaderProgram OpenGLMarcherShader;
    OpenGLMarcherShader.attachVertexShader(TOSTRING(SHADER_PATH) "oglMarcher.vert");
    OpenGLMarcherShader.attachFragmentShader(TOSTRING(SHADER_PATH) "oglMarcher.frag");
    OpenGLMarcherShader.compile();

    //set clearscreen color to a nice navy blue
    //glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    scene = new Scene();

    marcher = new RayMarcher(1000, 800);
    marchingCubes = new MarchingCubes(1000, 800, scene, &shaderProgMarchingCubes);
    oglMarcher = new OpenGLMarcher(1000, 800, scene, &OpenGLMarcherShader);

    glfwSetErrorCallback(error_callback);

    while (!glfwWindowShouldClose(window))
    {
        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        imGuiTest();

        if (oglMarcher->dirty && selectedRenderBackend == RenderType::SPHERE_MARCHING_GPU) {
            oglMarcher->linearize();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        switch (selectedRenderBackend)
        {
        case RenderType::SPHERE_MARCHING_CPU:
            screen->render();
            break;
        case RenderType::CUBE_MARCHING_CPU:
            marchingCubes->render();
            break;
        case RenderType::SPHERE_MARCHING_GPU:
            oglMarcher->render();
            break;
        default:
            throw std::logic_error("render backend of chosen type is not implemented");
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
    OpenGLMarcherShader.destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete screen;
    delete marcher;
    delete scene;
    delete oglMarcher;
    delete marchingCubes;

    return 0;
}
