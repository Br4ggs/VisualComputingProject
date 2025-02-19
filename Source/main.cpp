#include <iostream>
#include "header/main.h"
#include "header/MarchingCubes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

// Implicit function: Sphere equation
float implicitFunction(float x, float y, float z, float r) {
    return x * x + y * y + z * z - r * r;
}

// Grid parameters
const int gridSize = 200;
const float gridSpacing = 0.01f;
const float sphereRadius = 0.5f;

int main() {
    std::cout << "Starting Marching Cubes..." << std::endl;

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

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

    std::vector<glm::vec3> vertices;
    marchingCubes(grid, vertices);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(0);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}
