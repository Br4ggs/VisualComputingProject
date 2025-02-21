#include <fstream>
#include <iostream>
#include "header/main.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

typedef struct Shader {
    int size;
    char *content;
} ShaderFile;


typedef struct Vertex {
    float pos[3];
    float tex[2];
} VertexInfo;

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

    return 0;
}

// Function to generate a checkered texture
std::vector<unsigned char> generateCheckeredTexture(int width, int height, int checkSize) {
    std::vector<unsigned char> textureData(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int checkX = x / checkSize;
            int checkY = y / checkSize;
            bool isWhite = (checkX + checkY) % 2 == 0;

            int index = (y * width + x) * 3;
            textureData[index] = isWhite ? 255 : 0;
            textureData[index + 1] = isWhite ? 255 : 0;
            textureData[index + 2] = isWhite ? 255 : 0;
        }
    }

    return textureData;
}

GLuint loadCheckeredTexture(int texture_height, int texture_width, int checker_size) {
    std::vector<unsigned char> imageData = generateCheckeredTexture(texture_width, texture_height, checker_size);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Upload texture data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

    // Set texture parameters (important for proper rendering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

    return textureID;
}

int main()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
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


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "VisualComputingProject", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    int version = gladLoadGL();
    if (version == 0) {
        std::cout << "Glad could not bind OpenGL context\n";
        return 1;
    }

    glfwSwapInterval(1);

    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }

    constexpr size_t n_vertices = 4;

    VertexInfo vertices[n_vertices] =
        {
            {{ -1.0, -1.0,  -0.5 }, {  0.0,  0.0}},
            {{ -1.0,  1.0,   0.5 }, {  1.0,  0.0}},
            {{  1.0,  1.0,  -0.5 }, {  1.0,  1.0}},
            {{  1.0, -1.0,   0.5 }, {  0.0,  1.0}},
        };

    unsigned int triangle_elements[] = 
        {
            0, 1, 2, /* first triangle vertices */
            0, 2, 3  /* second triangle vertices */
        };

    glViewport(0, 0, 800, 800);

    GLuint shaderProgram = glCreateProgram();

    int success;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader->content, NULL);
    glCompileShader(vertexShader);
    glAttachShader(shaderProgram, vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "error compiling vertex shader \n";
        return 1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader->content, NULL);
    glCompileShader(fragmentShader);
    glAttachShader(shaderProgram, fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "error compiling fragment shader \n";
        return 1;
    }

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);

    GLuint vertex_buffer_id;
    GLuint element_buffer_id;

    glGenBuffers(1, &vertex_buffer_id);
    glGenBuffers(1, &element_buffer_id);

    glBindVertexArray(vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_elements), triangle_elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void *)offsetof(VertexInfo, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void *)offsetof(VertexInfo, tex));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint texture_id = loadCheckeredTexture(800, 800, 30);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    int resolution_location = glGetUniformLocation(shaderProgram, "windowResolution");

    while (!glfwWindowShouldClose(window))
    {
        int screen_width, screen_height;
        glfwGetFramebufferSize(window, &screen_width, &screen_height);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniform2f(resolution_location, screen_width, screen_height);
        glDrawElements(GL_TRIANGLES, sizeof(triangle_elements) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(vertex_shader->content);
    free(vertex_shader);
    free(fragment_shader->content);
    free(fragment_shader);

    glDeleteVertexArrays(1, &vertex_array_id);
    glDeleteBuffers(1, &vertex_buffer_id);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
