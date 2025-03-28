#include "OpenGLStrategy.h"
#include "shaderProgram.h"
#include "IRenderStrategy.h"
#include "scene.h"
#include <iostream>

constexpr int N_VERTICES = 4;
constexpr int N_ELEMENTS  = N_VERTICES * 3;

typedef struct Vertex
{
    glm::vec3 pos;
    glm::vec3 col;
} Vertex;

constexpr Vertex vertices[N_VERTICES] =
    {
        { { -1.0f, -1.0f, 0.0f }, { 1.f, 0.f, 0.f } },
        { {  1.0f, -1.0f, 0.0f }, { 0.f, 1.f, 0.f } },
        { {  1.0f,  1.0f, 0.0f }, { 0.f, 0.f, 1.f } },
        { { -1.0f,  1.0f, 0.0f }, { 1.f, 1.f, 1.f } },
    };

constexpr GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };

OpenGLStrategy::OpenGLStrategy(RenderStrategyType type)
: IRenderStrategy(type)
{
    setup();
}

OpenGLStrategy::~OpenGLStrategy()
{
}

void OpenGLStrategy::render(Scene *scene, GLFWwindow* window)
{
    shaderProgram.use();
}

void OpenGLStrategy::switchedStrategy()
{
    glBindVertexArray(VAOID);
    shaderProgram.use();
}

void OpenGLStrategy::setup()
{
    glGenBuffers(1, &uboID);

    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);

    std::cout << "created and bound vertex array object\n";

    shaderProgram.attachVertexShader("./build/oglMarcher.vert");
    std::cout << "alsdkjfalskdfj\n";
    shaderProgram.attachFragmentShader("./build/oglMarcher.frag");
    shaderProgram.compile();
    shaderProgram.use();

    std::cout << "using correct shader program for setup\n";

    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertices_id;
    glGenBuffers(1, &vertices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    unsigned int shaderProgramInt = shaderProgram.getRawShaderProgram();
    const GLint vpos_location = glGetAttribLocation(shaderProgramInt, "vPos");
    const GLint vcol_location = glGetAttribLocation(shaderProgramInt, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));
    glBindVertexArray(0);
}

void OpenGLStrategy::drawUI(bool &dirty)
{
}

void OpenGLStrategy::dirtyUpdate()
{
}
