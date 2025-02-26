#include "header/scene.h"

#include "header/sdfSphere.h"
#include "header/sdfBox.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

//TODO:
//start looking at imgui integration into scene class

//scene ui
//-settings for camera & lighting
//create a tree node for each object in the scene (make it contain a sphere and a box

Scene::Scene()
{
    sphere.setScale(glm::vec3(1.5f, 0.75f, 1.0f));

    //glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -3.14f / 3.0f, glm::vec3(1.0f, 1.0, 1.0f));
    //sphere.setRotation(rotation);

    box.setPosition(glm::vec3(0.5f, 0.5f, 0.0f));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -3.14f / 4.0f, glm::vec3(1.0f, 1.0, 1.0f));
    box.setRotation(rotation);

    box.setScale(glm::vec3(0.75f, 0.15f, 0.5f));
}

void Scene::drawUI()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        //camera position
        ImGui::PushID(1);
        ImGui::InputFloat3("Position", camPosf);
        ImGui::PopID();

        //camera lookat direction
        ImGui::InputFloat3("Look at", lookAtf);
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        //position
        ImGui::PushID(2);
        ImGui::InputFloat3("Position", lightPosf);
        ImGui::PopID();

        //color
        ImGui::ColorEdit3("Color", specColorf);
    }
}

glm::vec2 Scene::map(glm::vec3 point) const
{
    //mod3(&point, glm::vec3(5, 5, 5));
    //point = repeat(point, glm::vec3(2, 2, 2));

    //a plane
    //float planeDist = sdfPlane(point, glm::vec3(0, 1, 0), 3.0);
    //float planeID = 2.0;
    //glm::vec2 plane = glm::vec2(planeDist, planeID);

    //a single sphere
    //float sphereDist = sdfSphere(point, 1.3);
    //float sphereID = 1.0;
    //glm::vec2 sphere = glm::vec2(sphereDist, sphereID);

    //a cylinder, placed higher than the rest of the objects
    //glm::vec3 pc = point;
    //pc.y -= 4.0;
    //float cylinderDist = sdfCylinder(glm::vec3(pc.y, pc.x, pc.z), 1, 2); //rotate the cylinder by swapping the components around
    //float cylinderID = 1.0;
    //glm::vec2 cylinder(cylinderDist, cylinderID);

    glm::vec2 res = box.sdf(point);
    res = sdfUnion(res, sphere.sdf(point));
    //res = sdfDifference(res, sphere);
    //res = sdfUnion(res, cylinder);
    //res = sdfUnion(res, plane);

    return res;
}

glm::vec3 Scene::getCamPos() const
{
    return glm::vec3(camPosf[0], camPosf[1], camPosf[2]);
}

glm::vec3 Scene::getLookAt() const
{
    return glm::vec3(lookAtf[0], lookAtf[1], lookAtf[2]);
}

glm::vec3 Scene::getLightPos() const
{
    return glm::vec3(lightPosf[0], lightPosf[1], lightPosf[2]);
}

glm::vec3 Scene::getSpecColor() const
{
    return glm::vec3(specColorf[0], specColorf[1], specColorf[2]);
}

float Scene::sdfCylinder(glm::vec3 point, float radius, float height) const
{
    float d = glm::length(glm::vec2(point.x, point.z)) - radius;
    d = glm::max(d, abs(point.y) - height);
    return d;
}

float Scene::sdfSphere(glm::vec3 point, float radius) const
{
    return glm::length(point) - radius;
}

float Scene::sdfPlane(glm::vec3 point, glm::vec3 normal, float distanceFromOrigin) const
{
    return glm::dot(point, normal) + distanceFromOrigin;
}

glm::vec2 Scene::sdfUnion(glm::vec2 obj1, glm::vec2 obj2) const
{
    return (obj1.x < obj2.x) ? obj1 : obj2;
}

glm::vec2 Scene::sdfIntersect(glm::vec2 obj1, glm::vec2 obj2) const
{
    return (obj1.x > obj2.x) ? obj1 : obj2;
}

//max(obj1, -obj2)
glm::vec2 Scene::sdfDifference(glm::vec2 obj1, glm::vec2 obj2) const
{
    if (obj1.x > -obj2.x)
    {
        return obj1;
    }
    else
    {
        obj2.x *= -1;
        return obj2;
    }
}

glm::vec3 Scene::repeat(glm::vec3 point, glm::vec3 scale) const
{
    glm::vec3 q = point - scale * glm::round(point / scale);
    return q;
}