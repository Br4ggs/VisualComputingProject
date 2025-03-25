#include "header/scene.h"

#include "header/addOperatorModal.h"
#include "header/addPrimitiveModal.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include <limits>
#include <algorithm>

Scene::Scene()
{
    dirty = false;
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
        //list of current objects in the scene
        for (int i = 0; i < objects.size(); i++)
        {
            IDrawable* obj = objects[i];

            ImGui::PushID(i);

            objectNodeWithDelete(obj);

            ImGui::PopID();
        }

        static AddPrimitiveModal addPrimitiveModal;

        if (ImGui::Button("Add primitive"))
            ImGui::OpenPopup("Add primitive");

        addPrimitiveModal.drawUI(*this);

        static AddOperatorModal addOperatorModal;

        if (ImGui::Button("Add operator"))
            ImGui::OpenPopup("Add operator");

        addOperatorModal.drawUI(*this);
    }
}

std::pair<float, glm::vec3> Scene::map(glm::vec3 point) const
{
    std::pair<float, glm::vec3> res(std::numeric_limits<float>::infinity(), glm::vec3(1.0f));
    for (int i = 0; i < objects.size(); i++)
    {
        res = sdfUnion(res, objects[i]->sdf(point));
    }

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

void Scene::objectNodeWithDelete(IDrawable* obj)
{
    if (ImGui::TreeNode(obj->getName()))
    {
        ImGui::SameLine();
        if (ImGui::SmallButton("delete"))
        {
            deleteAsParent(obj);
            obj = nullptr;
        }

        ImGui::PushID(1);
        if (obj != nullptr)
            obj->drawUI();
        ImGui::PopID();

        ImGui::TreePop();
    }
    else
    {
        ImGui::SameLine();
        if (ImGui::SmallButton("delete"))
        {
            deleteAsParent(obj);
        }
    }
}

void Scene::deleteAsParent(IDrawable* obj)
{
    std::vector<IDrawable*> children = obj->detachChildren();
    if (children.size() > 0)
    {
        for (int i = 0; i < children.size(); i++)
        {
            //add to objects
            objects.push_back(children[i]);
        }
    }

    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
    delete obj;
    obj = nullptr;
}

std::pair<float, glm::vec3> Scene::sdfUnion(std::pair<float,
                                            glm::vec3> obj1,
                                            std::pair<float,
                                            glm::vec3> obj2) const
{
    return (obj1.first < obj2.first) ? obj1 : obj2;
}
