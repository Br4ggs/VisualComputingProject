#include "OpenGLMarcher.h"
#include "imgui.h"
#include "linearizeScene.h"
#include "types.h"

#define N_VERTICES 4
#define N_ELEMENTS N_VERTICES * 3

typedef struct Vertex
{
    glm::vec3 pos;
    glm::vec3 col;
} Vertex;

static const Vertex vertices[N_VERTICES] =
    {
        { { -1.0f, -1.0f, 0.0f }, { 1.f, 0.f, 0.f } },
        { {  1.0f, -1.0f, 0.0f }, { 0.f, 1.f, 0.f } },
        { {  1.0f,  1.0f, 0.0f }, { 0.f, 0.f, 1.f } },
        { { -1.0f,  1.0f, 0.0f }, { 1.f, 1.f, 1.f } },
    };

static const GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };

bool OpenGLMarcher::dirty = false;

OpenGLMarcher:: OpenGLMarcher(unsigned int width,
                              unsigned int height,
                              Scene *scene,
                              ShaderProgram *shaderProgram)
:width(width), height(height), shaderProgram(shaderProgram), scene(scene)
{
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

void OpenGLMarcher::linearize()
{
    linearScene.clear();
    linearizeScene(scene, linearScene);

    for (auto obj : linearScene) {
        printLinearCSGTreeNode(obj);
    }

    OpenGLMarcher::dirty = false;
}

void OpenGLMarcher::render(int width, int height)
{
    if (linearScene.size() == 0) return;
    if (dirty) linearize();

    glBindVertexArray(VAOID);

    shaderProgram->use();

    unsigned int shaderProgramInt = shaderProgram->getRawShaderProgram();

    // TODO: nodes is hardcoded now, this needs to come from the linearization
    int maxNodes = 128;

    assert(linearScene.size() <= maxNodes);

    glm::vec3 cam_pos = scene->getCamPos();
    const GLint cam_pos_location = glGetUniformLocation(shaderProgramInt, "u_camera_position");
    glUniform3f(cam_pos_location, cam_pos[0], cam_pos[1], cam_pos[2]);

    glm::vec3 light_pos = scene->getLightPos();
    const GLint light_pos_location = glGetUniformLocation(shaderProgramInt, "u_light_position");
    glUniform3f(light_pos_location, light_pos[0], light_pos[1], light_pos[2]);

    glm::vec3 look_at = scene->getLookAt();
    const GLint look_at_location = glGetUniformLocation(shaderProgramInt, "u_look_at");
    glUniform3f(look_at_location, look_at[0], look_at[1], look_at[2]);

    size_t dataSize = sizeof(LinearCSGTreeNode) * maxNodes;
    glGenBuffers(1, &uboID);
    glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    glBufferData(GL_UNIFORM_BUFFER, dataSize, linearScene.data(), GL_DYNAMIC_DRAW);

    const GLint loop_length_location = glGetUniformLocation(shaderProgramInt, "loop_length");
    glUniform1f(loop_length_location, linearScene.size());

    const GLint window_dim_location = glGetUniformLocation(shaderProgramInt, "window_dimensions");
    glUniform2f(window_dim_location, width, height);

    GLuint blockIndex = glGetUniformBlockIndex(shaderProgramInt, "CSGBuffer");
    glUniformBlockBinding(shaderProgramInt, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
