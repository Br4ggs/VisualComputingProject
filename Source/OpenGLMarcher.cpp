#include "OpenGLMarcher.h"
#include "imgui.h"
#include "linearizeScene.h"
#include "types.h"
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

OpenGLMarcher::OpenGLMarcher(unsigned int width,
                             unsigned int height,
                             Scene *scene,
                             ShaderProgram *shaderProgram)
    :width(width),
     height(height),
     shaderProgram(shaderProgram),
     scene(scene)
{
    glGenBuffers(1, &uboID);

    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);

    shaderProgram->use();

    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertices_id;
    glGenBuffers(1, &vertices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    unsigned int shaderProgramInt = shaderProgram->getRawShaderProgram();
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

OpenGLMarcher::~OpenGLMarcher()
{
}

void OpenGLMarcher::drawUI(bool &dirty)
{
    ImGui::SliderInt("max steps", &maxSteps, 10, 1000);
    ImGui::SliderFloat("max distance", &maxDist, 1, 20);

    if (ImGui::InputFloat("epsilon", &epsilon, 0.000001f, 0.0f, "%.8f")) {
        epsilon = glm::max(minEpsilon, epsilon);
    }

    ImGui::ColorEdit3("background color", colf);
}

void OpenGLMarcher::linearize(bool &dirty)
{
    linearScene.clear();
    linearizeScene(scene, linearScene);

    dirty = false;
}

void OpenGLMarcher::render(int width, int height)
{
    glBindVertexArray(VAOID);

    shaderProgram->use();

    unsigned int shaderProgramInt = shaderProgram->getRawShaderProgram();

    // TODO: nodes is hardcoded now, this needs to come from the linearization
    int maxNodes = 128;

    assert(linearScene.size() <= maxNodes);

    //uniform transfer
    glm::vec3 cam_pos = scene->getCamPos();
    shaderProgram->passUniform3floatVector("u_camera_position", cam_pos);

    glm::vec3 light_pos = scene->getLightPos();
    shaderProgram->passUniform3floatVector("u_light_position", light_pos);

    glm::vec3 look_at = scene->getLookAt();
    shaderProgram->passUniform3floatVector("u_look_at", look_at);

    shaderProgram->passUniformInt("u_max_steps", maxSteps);

    glm::vec3 specCol = scene->getSpecColor();
    shaderProgram->passUniform3floatVector("u_spec_color", specCol);

    shaderProgram->passUniformFloat("u_max_distance", maxDist);

    glm::vec3 background_color(colf[0], colf[1], colf[2]);
    shaderProgram->passUniform3floatVector("u_background_color", background_color);

    shaderProgram->passUniformFloat("u_epsilon", epsilon);

    shaderProgram->passUniformInt("u_fov", scene->fov);

    glm::vec3 ambientColor = scene->getAmbientColor();
    shaderProgram->passUniform3floatVector("u_ambient_color", ambientColor);

    shaderProgram->passUniformFloat("u_specular_strength", scene->specularStrength);

    shaderProgram->passUniformFloat("u_shininess", scene->shininess);

    //write linearized scene tree to buffer
    size_t dataSize = sizeof(LinearCSGTreeNode) * maxNodes;
    glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    glBufferData(GL_UNIFORM_BUFFER, dataSize, linearScene.data(), GL_DYNAMIC_DRAW);

    shaderProgram->passUniformFloat("loop_length", linearScene.size());

    glm::vec2 dimensions(width, height);
    shaderProgram->passUniform2floatVector("window_dimensions", dimensions);

    //binding
    GLuint blockIndex = glGetUniformBlockIndex(shaderProgramInt, "CSGBuffer");
    glUniformBlockBinding(shaderProgramInt, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
