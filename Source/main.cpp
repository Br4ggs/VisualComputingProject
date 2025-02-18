#include <fstream>
#include <iostream>
#include "header/main.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

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

    return 0;
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

    glfwSwapInterval(1); /* 1 = V-sync, 0 = ASAP */

    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }

    GLfloat vertices[] =
        {
            -0.8, -0.8, 0.0, 0.0, 0.0,
            -0.8,  0.8, 0.0, 1.0, 1.0,
             0.8,  0.8, 0.0, 0.0, 1.0,
             0.8, -0.8, 0.0, 1.0, 0.0,
        };
    unsigned int triangle_elements[] = 
        {
            0, 1, 2, /* first triangle vertices */
            0, 2, 3  /* second triangle vertices */
        };

    glViewport(0, 0, 800, 800);

    // === Creating shaders and attaching to program ===

    GLuint shaderProgram = glCreateProgram(); /* should hold all shaders */

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

    glLinkProgram(shaderProgram); /* can delete compiled shaders after linking */

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // === Setting up communication between CPU and GPU using buffers ===

    GLuint vertex_array_id; /* creates a placeholder object */
    glGenVertexArrays(1, /* number of arrays */
                      &vertex_array_id); /* placeholder for indices */

    GLuint vertex_buffer_id; /* is going to hold all vertices that exist */
    GLuint element_buffer_id; /* enables reuse of vertices by specifying which elements belong together */

    /* create the buffers */
    glGenBuffers(1, &vertex_buffer_id);
    glGenBuffers(1, &element_buffer_id);

    glBindVertexArray(vertex_array_id); /* set the current vertex array object */

    glBindBuffer(GL_ARRAY_BUFFER, /* target to which to bind */
                 vertex_buffer_id); /* name of buffer object */
    glBufferData(GL_ARRAY_BUFFER, /* target buffer to set data in */
                 sizeof(vertices), /* size in bytes of new data */
                 vertices, /* new data */
                 GL_STATIC_DRAW); /* usage pattern hint for memory layout */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_elements), triangle_elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, /* index of vertex attribute */
                          3, /* number of components per attribute, for triangle this is 3 */
                          GL_FLOAT, /* type of values in array */
                          GL_FALSE, /* should not be normalized anymore */
                          5 * sizeof(float), /* stride of the attributes */
                          (void*)0); /* pointer to first vertex attribute */
    glEnableVertexAttribArray(0); /* enable the array with index 0 (see above)*/

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    int head = 0;
    int size = 10;
    int* sample = (int*)malloc(size * sizeof(int));

    while (!glfwWindowShouldClose(window))
    {
        frameCount++;

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto timeDelta = std::chrono::duration<double>(currentTime - lastTime).count();

        if (timeDelta >= 1.0) // Every second
        {
            sample[head++] = frameCount;

            if (head == size) {
                head = 0;
                for (int i = 0; i < size; ++i) {
                    std::cout << sample[i] << " ";
                }
                std::cout << "\n";
            }

            frameCount = 0;
            lastTime = currentTime;
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); /* rgba for when colors are cleared */
        glClear(GL_COLOR_BUFFER_BIT); /* clear the color buffer */
        glUseProgram(shaderProgram);
        glBindVertexArray(vertex_array_id);
        glDrawElements(GL_TRIANGLES, /* primitive to render */
                       6, /* number of elements to render */
                       GL_UNSIGNED_INT, /* type of value in the 'indices' */
                       0); /* byte offset into the bound buffer, or pointer to location if no buffer is bound */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < head; ++i) {
        std::cout << sample[i] << " ";
    }
    std::cout << "\n";

    free(sample);
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
