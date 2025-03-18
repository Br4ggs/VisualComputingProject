#include "header/OpenGLMarcher.h"

#include "imgui.h"

#define N_VERTICES 4
#define N_ELEMENTS N_VERTICES * 3

typedef struct Vertex
{
    glm::vec3 pos;
    glm::vec3 col;
} Vertex;

struct LinearCSGTreeNode {
    int32_t op;
    int32_t shape;
    float _padding[2];
    glm::vec4 position;
    glm::vec4 dimensions;
};

static const Vertex vertices[N_VERTICES] =
    {
        { { -1.0f, -1.0f, 0.0f }, { 1.f, 0.f, 0.f } },
        { {  1.0f, -1.0f, 0.0f }, { 0.f, 1.f, 0.f } },
        { {  1.0f,  1.0f, 0.0f }, { 0.f, 0.f, 1.f } },
        { { -1.0f,  1.0f, 0.0f }, { 1.f, 1.f, 1.f } },
    };

static const GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };

enum CSGOperation {
    OP_UNI = 0,
    OP_INT = 1,
    OP_SUB = 2,
    NO_OP = 3,
};

enum CSGShape {
    SHAPE_SPHERE = 0,
    SHAPE_BOX = 1,
    NO_SHAPE = 2,
};

const std::vector<LinearCSGTreeNode> nodes = {
    { NO_OP, SHAPE_SPHERE, 0, 0, glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(2.0f) },
    { NO_OP, SHAPE_SPHERE, 0, 0, glm::vec4(0.0, 2.0, 0.0, 0.0), glm::vec4(1.0f) },
    { OP_INT, NO_SHAPE, 0, 0, glm::vec4(0), glm::vec4(0) },
};

OpenGLMarcher:: OpenGLMarcher(unsigned int width,
                              unsigned int height,
                              Scene *scene,
                              ShaderProgram *shaderProgram)
:width(width), height(height), shaderProgram(shaderProgram), scene(scene)
{
    int maxNodes = 128;
    size_t dataSize = sizeof(LinearCSGTreeNode) * maxNodes;
    glGenBuffers(1, &uboID);
    glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    glBufferData(GL_UNIFORM_BUFFER, dataSize, nodes.data(), GL_DYNAMIC_DRAW);
}

OpenGLMarcher::~OpenGLMarcher()
{
}

void OpenGLMarcher::drawUI()
{
    if (ImGui::InputInt("max steps", &maxSteps))
    {
        if (maxSteps < 1)
            maxSteps = 1;
    }

    if (ImGui::InputFloat("max distance", &maxDist))
    {
        if (maxDist < 1)
            maxDist = 1.0f;
    }

    if (ImGui::ColorEdit3("background color", colf))
    {
        backgroundColor = glm::vec3(colf[0], colf[1], colf[2]);
    }

    ImGui::InputDouble("fog creep", &fogCreep);
}

void OpenGLMarcher::render()
{
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    shaderProgram->use();

    unsigned int shaderProgramInt = shaderProgram->getRawShaderProgram();
    const GLint vpos_location = glGetAttribLocation(shaderProgramInt, "vPos");
    const GLint vcol_location = glGetAttribLocation(shaderProgramInt, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    const GLint window_dim_location = glGetUniformLocation(shaderProgramInt, "window_dimensions");
    glUniform2f(window_dim_location, width, height);

    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertices_id;
    glGenBuffers(1, &vertices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLuint blockIndex = glGetUniformBlockIndex(shaderProgramInt, "CSGBuffer");
    glUniformBlockBinding(shaderProgramInt, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
