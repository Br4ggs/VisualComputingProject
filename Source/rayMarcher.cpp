#include "header/rayMarcher.h"

#include "imgui.h"

RayMarcher::RayMarcher(unsigned int width, unsigned int height)
	:width(width), height(height)
{
	data = new unsigned char[width * height * 3];
}

RayMarcher::~RayMarcher()
{
	delete[] data;
	data = nullptr;
}

void RayMarcher::drawUI()
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

    if (ImGui::InputFloat("epsilon", &epsilon, 0.000001f, 0.0f, "%.8f")) {
        epsilon = glm::max(minEpsilon, epsilon);
    }

    if (ImGui::ColorEdit3("background color", colf))
    {
        backgroundColor = glm::vec3(colf[0], colf[1], colf[2]);
    }

    ImGui::InputDouble("fog creep", &fogCreep);
}

void RayMarcher::render(Scene* scene)
{
    this->scene = scene;
    glm::vec2 resolution(width, height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //normalize screen coordinates
            glm::vec2 fragCoord(j, i);
            glm::vec2 uv = (2.0f * fragCoord - resolution) / resolution.y;

            //direction vectors
            glm::vec3 rayOrigin = scene->getCamPos();
            glm::vec3 lookat = scene->getLookAt();
            glm::vec3 rayDirection = getCamera(rayOrigin, lookat) * glm::normalize(glm::vec3(uv.x, uv.y, FOV));

            //do rendering
            glm::vec3 background = backgroundColor;
            glm::vec3 color = glm::vec3(0.0f);

            //first component is the distance, second component is the material (color)
            std::pair<float, glm::vec3> object = rayMarch(rayOrigin, rayDirection);

            if (object.first < maxDist)
            {
                //render object using corresponding material
                glm::vec3 p = rayOrigin + rayDirection * object.first;
                glm::vec3 material = object.second;
                color += getLight(p, rayDirection, material);

                //add fog to mitigate ugly aliasing effects
                glm::vec3 bg = background - glm::max(0.95f * -rayDirection.y, 0.0f);
                color = glm::mix(color, bg, 1.0 - exp((float)fogCreep * object.first * object.first));
            }
            else
            {
                //render background
                color += background - glm::max(0.95f * -rayDirection.y, 0.0f);
            }

            //perform gamma correction
            color = glm::pow(color, glm::vec3(0.4545));

            //convert float to 8-bit color data for each separate channel
            unsigned int R = floor(color.x >= 1.0 ? 255 : color.x * 256.0);
            unsigned int G = floor(color.y >= 1.0 ? 255 : color.y * 256.0);
            unsigned int B = floor(color.z >= 1.0 ? 255 : color.z * 256.0);

            //set color
            data[((i * width) + j) * 3]     = R;
            data[((i * width) + j) * 3 + 1] = G;
            data[((i * width) + j) * 3 + 2] = B;
        }
    }
}

unsigned char* RayMarcher::getRenderData() const
{
    return &data[0];
}

glm::mat3 RayMarcher::getCamera(glm::vec3 rayOrigin, glm::vec3 lookAt) const
{
    glm::vec3 camForward = glm::normalize(lookAt - rayOrigin);
    glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
    glm::vec3 camUp = glm::cross(camForward, camRight);

    return glm::mat3(camRight, camUp, camForward);
}


glm::vec3 RayMarcher::getLight(glm::vec3 point, glm::vec3 rayDirection, glm::vec3 color) const
{
    glm::vec3 lightPos = scene->getLightPos();
    glm::vec3 ambientColor = scene->getAmbientColor();
    glm::vec3 specColor = scene->getSpecColor();

    glm::vec3 L = glm::normalize(lightPos - point);
    glm::vec3 N = getNormal(point);
    glm::vec3 V = -rayDirection;
    glm::vec3 R = glm::reflect(-L, N);

    //standard phong light model
    glm::vec3 ambient = color * ambientColor;
    glm::vec3 diffuse = color * glm::clamp(glm::dot(L, N), 0.0f, 1.0f);
    glm::vec3 specular = specColor * pow(glm::clamp(glm::dot(R, V), 0.0f, 1.0f), scene->shininess) * scene->specularStrength;

    //shadows
    float d = rayMarch(point + N * 0.02f, glm::normalize(lightPos)).first;
    if (d < glm::length(lightPos - point)) return ambient;

    return diffuse + ambient + specular;
}

//returns an approximate of the normal
glm::vec3 RayMarcher::getNormal(glm::vec3 point) const
{
    glm::vec2 e(epsilon, 0.0);

    float comp1 = scene->map(point - glm::vec3(e.x, e.y, e.y)).first;
    float comp2 = scene->map(point - glm::vec3(e.y, e.x, e.y)).first;
    float comp3 = scene->map(point - glm::vec3(e.y, e.y, e.x)).first;
    glm::vec3 n = glm::vec3(scene->map(point).first) - glm::vec3(comp1, comp2, comp3);
    return glm::normalize(n);
}

std::pair<float, glm::vec3> RayMarcher::rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection) const
{
    std::pair<float, glm::vec3> hit;
    std::pair<float, glm::vec3> object = std::make_pair(0.0f, glm::vec3(0.0f)); //?

    for (int i = 0; i < maxSteps; i++)
    {
        glm::vec3 p = rayOrigin +  rayDirection * object.first;
        hit = scene->map(p);
        object.first += hit.first;
        object.second = hit.second;

        if (abs(hit.first) < epsilon || object.first > maxDist) break;

        if (i == maxSteps - 1) //ugly hack but appears to work for plane parallelness
        {
            object.first = maxDist + 1;
        }
    }
    return object;
}
