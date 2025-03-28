#include "header/scene.h"

#include "header/addOperatorModal.h"
#include "header/addPrimitiveModal.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include <limits>
#include <algorithm>

Scene::Scene(GLFWwindow *window)
:window(window)
{
    inputController = new InputController(window, this);
    glfwSetWindowUserPointer(window, inputController);
    glfwSetScrollCallback(window, InputController::scrollCallbackWrapper);
}

Scene::~Scene()
{
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
}

void Scene::addRenderStrategy(std::unique_ptr<IRenderStrategy> strategy)
{
    if (activeStrategy == nullptr) { /* set the first strategy as active */
        activeStrategy = strategy.get();
    }
    strategies.push_back(std::move(strategy));
}

void Scene::setActiveStrategy(RenderStrategy::Type type)
{
    for (const auto &strategy : strategies) {
        if (strategy->getType() == type) {
            activeStrategy = strategy.get();
        }
    }

    throw std::logic_error("strategy is not found, abort");
}

void Scene::render()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(400, 680));

    bool dirty = false;

    ImGui::Begin("Settings");
    if (ImGui::CollapsingHeader("Controls"))
    {
        inputController->drawUI();
    }

    if (ImGui::CollapsingHeader("Rendering"))
    {


        if (ImGui::BeginCombo("rendering backend", "deeznuts"))
        {
            // TODO: switch backend here
            ImGui::EndCombo();
        }

        // TODO: draw ui from render strategy here

    }

    // TODO: call draw ui from scene here

    //call to Scene ui
    this->drawUI(dirty);

    // TODO: handle dirty scene for the chosen render strategy here

    ImGui::End();

    inputController->processInput();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activeStrategy->render(this, window);
}


void Scene::drawUI(bool& dirty)
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        //camera position
        ImGui::PushID(1);
        if (ImGui::InputFloat3("Position", camPosf)) dirty = true;
        ImGui::PopID();

        //camera lookat direction
        if (ImGui::InputFloat3("Look at", lookAtf)) dirty = true;

        ImGui::SliderInt("Field of view", &fov, 30, 120);
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        //position
        ImGui::PushID(2);
        if (ImGui::InputFloat3("Position", lightPosf)) dirty = true;
        ImGui::PopID();

        //color
        if (ImGui::ColorEdit3("Specular color", specColorf)) dirty = true;

        ImGui::ColorEdit3("Ambient color", ambientColorf);
        ImGui::SliderFloat("Specular strength", &specular_strength, 0.1, 0.9);
        ImGui::SliderFloat("Shininess", &shininess, 2.0, 128.0);
    }

    if (ImGui::CollapsingHeader("Geometry"))
    {
        //list of current objects in the scene
        for (int i = 0; i < objects.size(); i++)
        {
            IDrawable* obj = objects[i];

            ImGui::PushID(i);

            objectNodeWithDelete(obj, dirty);

            ImGui::PopID();
        }

        static AddPrimitiveModal addPrimitiveModal;

        if (ImGui::Button("Add primitive"))
            ImGui::OpenPopup("Add primitive");

        addPrimitiveModal.drawUI(*this, dirty);

        static AddOperatorModal addOperatorModal;

        if (ImGui::Button("Add operator"))
            ImGui::OpenPopup("Add operator");

        addOperatorModal.drawUI(*this, dirty);
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

void Scene::setCamPos(glm::vec3 pos)
{
    camPosf[0] = pos.x;
    camPosf[1] = pos.y;
    camPosf[2] = pos.z;
}

glm::vec3 Scene::getLookAt() const
{
    return glm::vec3(lookAtf[0], lookAtf[1], lookAtf[2]);
}

void Scene::setLookAt(glm::vec3 pos)
{
    lookAtf[0] = pos.x;
    lookAtf[1] = pos.y;
    lookAtf[2] = pos.z;
}

glm::vec3 Scene::getLightPos() const
{
    return glm::vec3(lightPosf[0], lightPosf[1], lightPosf[2]);
}

glm::vec3 Scene::getAmbientColor() const
{
    return glm::vec3(ambientColorf[0], ambientColorf[1], ambientColorf[2]);
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

void Scene::objectNodeWithDelete(IDrawable* obj, bool& dirty)
{
    if (ImGui::TreeNode(obj->getName()))
    {
        ImGui::SameLine();
        if (ImGui::SmallButton("delete"))
        {
            dirty = true;
            deleteAsParent(obj);
            obj = nullptr;
        }

        ImGui::PushID(1);
        if (obj != nullptr)
            obj->drawUI(dirty);
        ImGui::PopID();

        ImGui::TreePop();
    }
    else
    {
        ImGui::SameLine();
        if (ImGui::SmallButton("delete"))
        {
            dirty = true;
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

std::pair<float, glm::vec3> Scene::sdfUnion(std::pair<float, glm::vec3> obj1, std::pair<float, glm::vec3> obj2) const
{
    return (obj1.first < obj2.first) ? obj1 : obj2;
}
