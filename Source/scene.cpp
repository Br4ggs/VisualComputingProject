#include "header/scene.h"

#include "header/sdfSphere.h"
#include "header/sdfBox.h"
#include "header/opUnion.h"
#include "header/opIntersect.h"
#include "header/opDifference.h"

#include "header/addOperatorModal.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include <limits>
#include <iostream>
#include <algorithm>

//TODO:
//add class for union and difference
//add class for repeat operator?
//add ui for rotations?
//adding a thing as child of an existing operator?
//moving objects

//scene ui
//create a tree node for each object in the scene (make it contain a sphere and a box)

Scene::Scene()
{
    SDFBox* box = new SDFBox(glm::vec3(1.0f, 1.0f, 1.0f));
    SDFSphere* sphere = new SDFSphere(1.0f);
    SDFSphere* sphere2 = new SDFSphere(1.0f);
    SDFSphere* sphere3 = new SDFSphere(1.0f);

    box->setScale(glm::vec3(0.75f));

    sphere2->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    sphere3->setPosition(glm::vec3(1.0f, 0.0f, 1.0f));

    OpDifference* opDifference = new OpDifference(sphere, box);
    objects.push_back(opDifference);
    objects.push_back(sphere2);
    objects.push_back(sphere3);
}

Scene::~Scene()
{
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
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

    if (ImGui::CollapsingHeader("Geometry"))
    {
        const char* objectTypes[] = { "sphere", "cube" };
        static int selectedItem = 0;
        const char* preview = objectTypes[selectedItem];

        //list of current objects in the scene
        for (int i = 0; i < objects.size(); i++)
        {
            IDrawable* obj = objects[i];

            ImGui::PushID(i);
            obj->drawUI();

            ImGui::SameLine();
            if (ImGui::SmallButton("delete"))
            {
                std::cout << "deleting object" << std::endl;
                objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
                delete obj;
            }
            ImGui::PopID();
        }
        
        if (ImGui::BeginCombo("object", preview))
        {
            for (int n = 0; n < 2; n++)
            {
                const bool selected = (selectedItem == n);
                if (ImGui::Selectable(objectTypes[n], selected))
                    selectedItem = n;

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (ImGui::Button("Add new object"))
        {
            switch (selectedItem)
            {
            case 0: //sphere
            {
                std::cout << "adding sphere to scene" << std::endl;
                IDrawable* obj = new SDFSphere(1.0f);
                objects.push_back(obj);
                break;
            }
            case 1: //cube
            {
                std::cout << "adding cube to scene" << std::endl;
                IDrawable* obj = new SDFBox(glm::vec3(1.0f, 1.0f, 1.0f));
                objects.push_back(obj);
            }
            default:
                break;
            }
        }

        static AddOperatorModal addOperatorModal;

        if (ImGui::Button("Add operator"))
            ImGui::OpenPopup("Operator");

        addOperatorModal.drawUI(*this);
    }
}

std::pair<float, glm::vec3> Scene::map(glm::vec3 point) const
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

    std::pair<float, glm::vec3> res(std::numeric_limits<float>::max(), -1);
    for (int i = 0; i < objects.size(); i++)
    {
        res = sdfUnion(res, objects[i]->sdf(point));
    }

    //res = sdfUnion(res, sphere.sdf(point));
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

std::vector<IDrawable*> Scene::getObjects()
{
    return objects;
}

void Scene::addObject(IDrawable* obj)
{
    objects.push_back(obj);
}

void Scene::removeObject(IDrawable* obj)
{
    objects.erase(std::remove_if(objects.begin(), objects.end(), [obj](IDrawable* i) {return i == obj; }));
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

std::pair<float, glm::vec3> Scene::sdfUnion(std::pair<float, glm::vec3> obj1, std::pair<float, glm::vec3> obj2) const
{
    return (obj1.first < obj2.first) ? obj1 : obj2;
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