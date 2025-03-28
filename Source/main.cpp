#include "header/main.h"
#include <iostream>
#include "types.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//imgui related includes
#include "OpenGLStrategy.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include "header/shaderProgram.h"
#include "header/texturedScreenQuad.h"
#include "header/rayMarcher.h"
#include "header/marchingCubes.h"
#include "header/OpenGLMarcher.h"
#include "header/inputController.h"

int selectedRenderBackend = 2;

TexturedScreenQuad* screen = nullptr;
RayMarcher* marcher = nullptr;
MarchingCubes* marchingCubes = nullptr;
OpenGLMarcher* oglMarcher = nullptr;
Scene* scene = nullptr;
InputController* input = nullptr;

//unfortunately there is no way in glfw to poll scroll position, and since glfw is a C library only function
//callbacks are supported (no class instance methods). So this is a little hack to reroute the callback to our input class.
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (input == nullptr) return;

    input->processScrollEvent(xoffset, yoffset);
}

void drawUI()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(400, 680));

    bool dirty = false;

    ImGui::Begin("Settings");
    if (ImGui::CollapsingHeader("Controls"))
    {
        input->drawUI();
    }

    if (ImGui::CollapsingHeader("Rendering"))
    {

        //rendering backend dropdown
        const char* renderTypes[] = { "sphere tracing (cpu)", "marching cubes", "sphere tracing gpu"};
        const char* renderPreview = renderTypes[selectedRenderBackend];

        if (ImGui::BeginCombo("rendering backend", renderPreview))
        {
            for (int n = 0; n < 3; n++)
            {
                const bool selected = (selectedRenderBackend == n);
                if (ImGui::Selectable(renderTypes[n], selected))
                {
                    std::cout << "i was clicked" << std::endl;
                    selectedRenderBackend = n;
                    dirty = true;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
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
        case 2: // sphere marching opengl
            oglMarcher->drawUI(dirty);
        default:
            break;
        }
    }

    //call to Scene ui
    scene->drawUI(dirty);

    if (selectedRenderBackend == 1 && dirty)
    {
        marchingCubes->regenerateMarchingCubes();
    }
    if (selectedRenderBackend == 2 && dirty)
    {
        oglMarcher->linearize(dirty);
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
    shaderProgMarcher.attachVertexShader("./build/vertexScreenQuad.glsl");
    shaderProgMarcher.attachFragmentShader("./build/fragmentScreenQuad.glsl");
    shaderProgMarcher.compile();

    screen = new TexturedScreenQuad(&shaderProgMarcher);

    ShaderProgram shaderProgMarchingCubes;
    shaderProgMarchingCubes.attachVertexShader("./build/vertexMarchingCubes.glsl");
    shaderProgMarchingCubes.attachFragmentShader("./build/fragmentMarchingCubes.glsl");
    shaderProgMarchingCubes.compile();

    ShaderProgram OpenGLMarcherShader;
    OpenGLMarcherShader.attachVertexShader("./build/oglMarcher.vert");
    OpenGLMarcherShader.attachFragmentShader("./build/oglMarcher.frag");
    OpenGLMarcherShader.compile();

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    scene = new Scene();

    OpenGLStrategy *openglRenderStrategy = new OpenGLStrategy(
        RenderStrategyType::SPHERE_MARCHING_GPU
    );

    marcher = new RayMarcher(1000, 800);
    marchingCubes = new MarchingCubes(1000, 800, scene, &shaderProgMarchingCubes);
    oglMarcher = new OpenGLMarcher(1000,800, scene, &OpenGLMarcherShader);
    input = new InputController(window, scene);


    glEnable(GL_DEPTH_TEST);

    input = new InputController(window, scene);
    glfwSetScrollCallback(window, scrollCallback);

    while (!glfwWindowShouldClose(window))
    {
        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawUI();
        input->processInput();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

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
        case 2: //gpu-based sphere tracing
            oglMarcher->render(width, height);
            break;
        default:
            std::cout << "default\n";
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
