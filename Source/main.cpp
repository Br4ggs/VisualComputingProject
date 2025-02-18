#include <fstream>
#include <iostream>
#include "header/main.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//imgui related includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

typedef struct Shader {
    int size;
    char *content;
} ShaderFile;

int load_from_file(const char * const filename, ShaderFile* sf) {
    std::ifstream ifs = std::ifstream{filename, std::ifstream::in};

    if (!ifs) {
        std::cerr <<
            "issue loading shader from file: " <<
            filename;

        return 1;
    }

    ifs.seekg(0, ifs.end);
    int size = ifs.tellg();
    sf->size = size;
    sf->content = (char *)malloc(sizeof(char) * size + 1);
    ifs.seekg(0, ifs.beg);
    ifs.read(sf->content, size);
    ifs.close();
    sf->content[size] = '\0';
    std::cout << sf->content;

    return 0;
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

    GLfloat vertices[] =
    {
        -1., -1.,  0., /* bottom left */
         0.,  0.,  0., /* top */
         1., -1.,  0., /* bottom right */
    };

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

    ShaderFile* vertex_shader = (ShaderFile*)malloc(sizeof(ShaderFile));
    ShaderFile* fragment_shader = (ShaderFile*)malloc(sizeof(ShaderFile));

    if (load_from_file("./Source/shaders/simple.vert", vertex_shader)) {
        std::cerr << "error loading vertex shader from file";
        return 1;
    }

    if (load_from_file("./Source/shaders/simple.frag", fragment_shader)) {
        std::cerr << "error loading fragment shader from file";
        return 1;
    }

    glViewport(0, 0, 800, 800);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader->content, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader->content, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertex array and vertex buffer object to transfer vertex data from cpu to gpu
    //ordering is important here!
    GLuint vertexArray;
    GLuint vertexBuffer;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
        //draws triangle
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //render
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
