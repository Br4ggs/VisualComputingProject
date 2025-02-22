#include "header/main.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//imgui related includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include "header/shaderProgram.h"
#include "header/texturedScreenQuad.h"

float FOV = 1.0;
int maxSteps = 256;
float maxDist = 500;
float epsilon = 0.001;

//TODO
//add intersection function
//add difference function
//populate render class

glm::vec2 sdfUnion(glm::vec2 obj1, glm::vec2 obj2)
{
    return (obj1.x < obj2.x) ? obj1 : obj2;
}

glm::vec2 sdfIntersect(glm::vec2 obj1, glm::vec2 obj2)
{
    return (obj1.x > obj2.x) ? obj1 : obj2;
}

//3d modulo function
void mod3(glm::vec3* p, glm::vec3 size)
{
    p->x = glm::mod(p->x + size.x * 0.5f, size.x) - size.x * 0.5f;
    p->y = glm::mod(p->y + size.y * 0.5f, size.y) - size.y * 0.5f;
    p->z = glm::mod(p->z + size.z * 0.5f, size.z) - size.z * 0.5f;
}

//selects the largest component of the vector
float vmax(glm::vec3 v)
{
    return glm::max(glm::max(v.x, v.y), v.z);
}

//a sphere with radius r
float sdfSphere(glm::vec3 p, float r)
{
    return glm::length(p) - r;
}

float sdfBox(glm::vec3 p, glm::vec3 dim)
{
    glm::vec3 d = glm::abs(p) - dim;
    return glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0)));
}

float sdfCylinder(glm::vec3 p, float r, float height)
{
    float d = glm::length(glm::vec2(p.x, p.z)) - r;
    d = glm::max(d, abs(p.y) - height);
    return d;
}

//a plane with normal n some distance from the scene origin
float sdfPlane(glm::vec3 p, glm::vec3 n, float distanceFromOrigin)
{
    return glm::dot(p, n) + distanceFromOrigin;
}

glm::vec2 map(glm::vec3 p)
{
    //mod3(&p, glm::vec3(7, 7, 7));
    //a plane
    float planeDist = sdfPlane(p, glm::vec3(0, 1, 0), 3.0);
    float planeID = 2.0;
    glm::vec2 plane = glm::vec2(planeDist, planeID);

    //a single sphere
    float sphereDist = sdfSphere(p, 1.3);
    float sphereID = 1.0;
    glm::vec2 sphere = glm::vec2(sphereDist, sphereID);

    //a box
    float boxDist = sdfBox(p, glm::vec3(1, 1, 1));
    float boxID = 1.0;
    glm::vec2 box(boxDist, boxID);

    //a cylinder, placed higher than the rest of the objects
    glm::vec3 pc = p;
    pc.y -= 4.0;
    float cylinderDist = sdfCylinder(glm::vec3(pc.y, pc.x, pc.z), 1, 2); //rotate the cylinder by swapping the components around
    float cylinderID = 1.0;
    glm::vec2 cylinder(cylinderDist, cylinderID);

    glm::vec2 res = box;
    res = sdfIntersect(res, sphere);
    //res = sdfUnion(res, cylinder);
    res = sdfUnion(res, plane);
    return res;
}

glm::vec2 rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    glm::vec2 hit;
    glm::vec2 object(0,0); //?

    for (int i = 0; i < maxSteps; i++)
    {
        glm::vec3 p = rayOrigin + object.x * rayDirection;
        hit = map(p);
        object.x += hit.x;
        object.y = hit.y;

        if (abs(hit.x) < epsilon || object.x > maxDist) break;
    }
    return object;
}

//returns an approximate of the normal
glm::vec3 getNormal(glm::vec3 p)
{
    glm::vec2 e(epsilon, 0.0);

    float comp1 = map(p - glm::vec3(e.x, e.y, e.y)).x;
    float comp2 = map(p - glm::vec3(e.y, e.x, e.y)).x;
    float comp3 = map(p - glm::vec3(e.y, e.y, e.x)).x;
    glm::vec3 n = glm::vec3(map(p).x) - glm::vec3(comp1, comp2, comp3);
    return glm::normalize(n);
}

glm::vec3 getLight(glm::vec3 p, glm::vec3 rayDirection, glm::vec3 color)
{
    glm::vec3 lightPos(20.0, 40.0, -30.0);
    glm::vec3 L = normalize(lightPos - p);
    glm::vec3 N = getNormal(p);
    glm::vec3 V = -rayDirection;
    glm::vec3 R = glm::reflect(-L, N);

    //standard phong light model
    glm::vec3 specColor = glm::vec3(0.5);
    glm::vec3 specular = specColor * pow(glm::clamp(glm::dot(R, V), 0.0f, 1.0f), 10.0f);
    glm::vec3 diffuse = color * glm::clamp(glm::dot(L, N), 0.0f, 1.0f);
    glm::vec3 ambient = color * 0.05f;

    //shadows
    float d = rayMarch(p + N * 0.02f, glm::normalize(lightPos)).x;
    if (d < glm::length(lightPos - p)) return ambient;

    return diffuse + ambient + specular;
}

glm::vec3 getMaterial(glm::vec3 p, float id)
{
    glm::vec3 m(0.0);
    switch (int(id))
    {
    case 1:
        m = glm::vec3(0.9, 0.9, 0.0);
        break;
    case 2:
        //m = glm::vec3(0.0, 0.5, 0.5);
        m = glm::vec3(0.2 + 0.4 * glm::mod(floor(p.x) + floor(p.z), 2.0f)); //creates a checkerboard pattern
        break;
    }
    return m;
}

glm::mat3 getCamera(glm::vec3 rayOrigin, glm::vec3 lookAt)
{
    glm::vec3 camForward = glm::normalize(lookAt - rayOrigin);
    glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
    glm::vec3 camUp = glm::cross(camForward, camRight);

    return glm::mat3(camRight, camUp, camForward);
}

int main()
{
    std::cout << "Hello " << testmacro << std::endl;

    //glm test code; wowee a matrix inverse!
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 inverse = glm::inverse(matrix);

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //vertices of a triangle
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  //left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,   //right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f //top corner
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

    glViewport(0, 0, 1000, 800);

    ShaderProgram shaderProg;
    shaderProg.attachVertexShader("..\\..\\..\\vertexScreenQuad.glsl"); //since the executable is located in Source/out/build/x64-Debug
    shaderProg.attachFragmentShader("..\\..\\..\\fragmentScreenQuad.glsl");

    shaderProg.compile();

    TexturedScreenQuad screenquad(&shaderProg);

    //vertex array and vertex buffer object to transfer vertex data from cpu to gpu
    //ordering is important here!
    //GLuint vertexArray;
    //GLuint vertexBuffer;
    //glGenVertexArrays(1, &vertexArray);
    //glGenBuffers(1, &vertexBuffer);

    //glBindVertexArray(vertexArray);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    ////unbind buffers
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //set clearscreen color to a nice navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    unsigned char* testTex = new unsigned char[1000 * 800 * 3];
    glm::vec2 resolution(1000, 800);

    while (!glfwWindowShouldClose(window))
    {
        //do magical raymarching here
        for (int i = 0; i < 800; i++) //height
        {
            for (int j = 0; j < 1000; j++) //width
            {
                //normalize screen coordinates
                glm::vec2 fragCoord(j, i);
                glm::vec2 uv = (2.0f * fragCoord - resolution) / resolution.y;

                glm::vec3 rayOrigin(6.0, 6.0, -3.0);
                glm::vec3 lookat(0, 0, 0);
                glm::vec3 rayDirection = getCamera(rayOrigin, lookat) * glm::normalize(glm::vec3(uv.x, uv.y, FOV));

                //do rendering
                glm::vec3 background(0.5, 0.8, 0.9);
                glm::vec3 color(0, 0, 0);
                glm::vec2 object = rayMarch(rayOrigin, rayDirection);
                if (object.x < maxDist)
                {
                    glm::vec3 p = rayOrigin + object.x * rayDirection;
                    glm::vec3 material = getMaterial(p, object.y);
                    //color += 3.0f / object.x;
                    color += getLight(p, rayDirection, material);
                    //add fog to mitigate ugly aliasing effects
                    color = glm::mix(color, background, 1.0 - exp(-0.0008 * object.x * object.x));
                }
                else
                {
                    //render background
                    color += background - glm::max(0.95f * rayDirection.y, 0.0f);
                }

                //perform gamma correction
                color = glm::pow(color, glm::vec3(0.4545));

                unsigned int R = floor(color.x >= 1.0 ? 255 : color.x * 256.0);
                unsigned int G = floor(color.y >= 1.0 ? 255 : color.y * 256.0);
                unsigned int B = floor(color.z >= 1.0 ? 255 : color.z * 256.0);

                //set color
                testTex[((i * 1000) + j) * 3]     = R; //R
                testTex[((i * 1000) + j) * 3 + 1] = G; //G
                testTex[((i * 1000) + j) * 3 + 2] = B; //B
            }
        }
        screenquad.writeToTexture(1000, 800, &testTex[0]);

        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        //draws triangle
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenquad.render();

        //glBindVertexArray(vertexArray);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //render
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] testTex;

    screenquad.destroy();
    shaderProg.destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
