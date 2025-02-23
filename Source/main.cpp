#include <iostream>
#include "header/main.h"
#include "header/Functions.h"

#include <fstream>

#include "header/MarchingCubes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::translate and glm::perspective
#include <glm/gtc/type_ptr.hpp>          // for glm::value_ptr
#include <vector>

// Implicit function: Sphere equation
float implicitFunction(float x, float y, float z, float r) {
    return x * x + y * y + z * z - r * r;
}

typedef struct Shader {
    int size;
    char *content;
} ShaderFile;

int load_from_file(const char *filename, ShaderFile* sf) {
    std::ifstream ifs(filename, std::ifstream::in | std::ifstream::binary);
    if (!ifs) {
        std::cerr << "Error loading shader file: " << filename << std::endl;
        return 1;
    }
    ifs.seekg(0, std::ifstream::end);
    int size = (int)ifs.tellg();
    sf->size = size;
    sf->content = static_cast<char *>(malloc(sizeof(char) * (size + 1)));
    ifs.seekg(0, std::ifstream::beg);
    ifs.read(sf->content, size);
    ifs.close();
    sf->content[size] = '\0';
    return 0;
}

// Grid parameters
const int gridSize = 300;
const float gridSpacing = 0.01f;
const float sphereRadius = 0.5f;

static void checkShaderCompile(GLuint shader) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Error:\n" << infoLog << std::endl;
    }
}

static void checkProgramLink(GLuint program) {
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program Linking Error:\n" << infoLog << std::endl;
    }
}

int main() {
    std::cout << "Starting Marching Cubes..." << std::endl;

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 800, "Marching Cubes", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    ShaderFile vsFile, fsFile;
    if (load_from_file("./shaders/marching_cubes.vert", &vsFile)) {
        std::cerr << "Could not load vertex shader file.\n";
        return 1;
    }

    if (load_from_file("./shaders/marching_cubes.frag", &fsFile)) {
        std::cerr << "Could not load fragment shader file.\n";
        return 1;
    }

    GLuint shaderProgram = glCreateProgram();

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsFile.content, NULL);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader);
    glAttachShader(shaderProgram, vertexShader);

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsFile.content, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link
    glLinkProgram(shaderProgram);
    checkProgramLink(shaderProgram);

    // Cleanup file data
    free(vsFile.content);
    free(fsFile.content);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<std::vector<std::vector<GridPoint>>> grid(gridSize,
        std::vector<std::vector<GridPoint>>(gridSize, std::vector<GridPoint>(gridSize)));

    // Fill grid with function values
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                float px = (x - gridSize / 2) * gridSpacing;
                float py = (y - gridSize / 2) * gridSpacing;
                float pz = (z - gridSize / 2) * gridSpacing;
                grid[x][y][z] = { glm::vec3(px, py, pz), implicitFunction(px, py, pz, sphereRadius) };
            }
        }
    }

    std::vector<Vertex> vertices;
    marchingCubes(grid, vertices);
    std::cout << "MC produced " << vertices.size() << " vertices.\n";

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // layout (location=0) => position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glDisable(GL_CULL_FACE);


    // layout (location=1) => normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // ------------------------------------------------
    //  Main Loop
    // ------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear
        glViewport(0,0,800,800);
        glClearColor(0.1f,0.1f,0.15f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Build basic transform (model/view/proj)
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-2));
        glm::mat4 proj  = glm::perspective(
            glm::radians(45.0f),
            1.0f, // aspect ratio = 800/800
            0.1f,
            100.0f
        );

        // Pass them as uniforms
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc  = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc  = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));

        // Light/material uniforms
        glm::vec3 lightPos   = glm::vec3(1.2f, 1.0f, 2.0f);
        glm::vec3 viewPos    = glm::vec3(0.0f, 0.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 objectColor= glm::vec3(0.8f, 0.8f, 0.9f);

        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"),   1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"),    1, &viewPos[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, &lightColor[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"),1, &objectColor[0]);

        // Draw the triangles
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}