#include <iostream>
#include "header/main.h"
#include "header/Functions.h"

#include <fstream>
#include <imgui.h>

#include "header/MarchingCubes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::translate and glm::perspective
#include <glm/gtc/type_ptr.hpp>          // for glm::value_ptr
#include <vector>

#include "header/imgui_layer.h"

// Implicit function: Sphere equation
float implicitFunction(float x, float y, float z, float r) {
    return x * x + y * y + z * z - r * r;
}

typedef struct Shader {
    int size;
    char *content;
} ShaderFile;

int load_from_file(const char *filename, ShaderFile *sf) {
    std::ifstream ifs(filename, std::ifstream::in | std::ifstream::binary);
    if (!ifs) {
        std::cerr << "Error loading shader file: " << filename << std::endl;
        return 1;
    }
    ifs.seekg(0, std::ifstream::end);
    int size = (int) ifs.tellg();
    sf->size = size;
    sf->content = static_cast<char *>(malloc(sizeof(char) * (size + 1)));
    ifs.seekg(0, std::ifstream::beg);
    ifs.read(sf->content, size);
    ifs.close();
    sf->content[size] = '\0';
    return 0;
}

// Grid parameters
const int gridSize = 30;
const float gridSpacing = 0.1f;
std::vector<std::vector<std::vector<GridPoint> > > grid(gridSize,
                                                        std::vector<std::vector<GridPoint> >(
                                                            gridSize, std::vector<GridPoint>(gridSize)));
std::vector<Vertex> vertices;
GLuint VAO, VBO;
static float sphereRadius = 0.5f;  // Default size
static float prevSphereRadius = sphereRadius;  // Store last value to detect changes

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

void regenerateMarchingCubes() {
    std::cout << "Regenerating Marching Cubes with sphere size: " << sphereRadius << std::endl;

    // Refill the grid with new values
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                float px = (x - gridSize / 2) * gridSpacing;
                float py = (y - gridSize / 2) * gridSpacing;
                float pz = (z - gridSize / 2) * gridSpacing;
                grid[x][y][z] = {glm::vec3(px, py, pz), implicitFunction(px, py, pz, sphereRadius)};
            }
        }
    }

    // Regenerate vertices
    vertices.clear();
    marchingCubes(grid, vertices);
    std::cout << "MC produced " << vertices.size() << " vertices.\n";

    // Update GPU buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}


int main() {
    std::cout << "Starting Marching Cubes..." << std::endl;
    // Add these near your existing ImGui variables
    static float sphereScale = 1.0f;        // how much to scale the sphere
    static int colorIndex = 0;             // 0 = Red, 1 = Green, 2 = Blue


    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Marching Cubes", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    ImGui_Init(window);

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

    // Fill grid with function values
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                float px = (x - gridSize / 2) * gridSpacing;
                float py = (y - gridSize / 2) * gridSpacing;
                float pz = (z - gridSize / 2) * gridSpacing;
                grid[x][y][z] = {glm::vec3(px, py, pz), implicitFunction(px, py, pz, sphereRadius)};
            }
        }
    }

    marchingCubes(grid, vertices);
    std::cout << "MC produced " << vertices.size() << " vertices.\n";

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    // layout (location=0) => position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    glDisable(GL_CULL_FACE);


    // layout (location=1) => normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // ------------------------------------------------
    //  Main Loop
    // ------------------------------------------------
    float spherePos[3] = {0.0f, 0.0f, 0.0f};

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_NewFrame();
        {
            ImGui::Begin("Sphere Controls");

            // Existing position controls
            ImGui::SliderFloat3("Position", spherePos, -1.0f, 1.0f);

            // New: Scale slider
            if (ImGui::SliderFloat("Sphere Size", &sphereRadius, 0.1f, 2.0f)) {
                if (sphereRadius != prevSphereRadius) {
                    prevSphereRadius = sphereRadius;  // Update last value
                    regenerateMarchingCubes();        // Call the function to update geometry
                }
            }

            // New: Color combo
            const char* colorOptions[] = {"Red", "Green", "Blue"};
            ImGui::Combo("Color", &colorIndex, colorOptions, IM_ARRAYSIZE(colorOptions));

            ImGui::End();
        }


        // Clear
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Build basic transform (model/view/proj)
        glm::mat4 model = glm::mat4(1.0f);
        // Apply translation from ImGui
        model = translate(model, glm::vec3(spherePos[0], spherePos[1], spherePos[2]));

        glm::mat4 view = translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float) displayW / (float) displayH, 0.1f, 100.0f);

        // Pass them as uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

        // Light/material uniforms
        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 objectColor;
        switch (colorIndex) {
            case 0: // Red
                objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
            case 1: // Green
                objectColor = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
            case 2: // Blue
                objectColor = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
            default:
                objectColor = glm::vec3(0.8f, 0.8f, 0.9f);
            break;
        }


        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &viewPos[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, &lightColor[0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(objectColor));

        // Draw the triangles
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());

        ImGui_Render();

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_Shutdown();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
