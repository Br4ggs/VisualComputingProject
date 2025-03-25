#include "header/worldObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include "scene.h"
#include "imgui.h"

WorldObject::WorldObject()
{
	float radiansX = glm::radians(rotf[0]);
	float radiansY = glm::radians(rotf[1]);
	float radiansZ = glm::radians(rotf[2]);

	glm::mat4 matX = glm::rotate(glm::mat4(1.0f), radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matY = glm::rotate(glm::mat4(1.0f), radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 matZ = glm::rotate(glm::mat4(1.0f), radiansZ, glm::vec3(0.0f, 0.0f, 1.0f));

	setRotation(matX * matY * matZ);

	setScale(glm::vec3(sclf[0], sclf[1], sclf[2]));
}

glm::vec3 WorldObject::getScale()
{
	return scale;
}

glm::vec3 WorldObject::getPosition()
{
	return glm::vec3(posf[0], posf[1], posf[2]);
}

void WorldObject::drawUI()
{
	//TODO: make the scene dirty when something changes
	if (ImGui::SliderFloat3("Position", posf, -1.0, 1.0))
	{
	}

	if (ImGui::SliderFloat3("Rotation", rotf, -1.0, 1.0))
	{
	}

	if (ImGui::SliderFloat("Rotation Angle", &rotation_angle_deg, 0.0, 360.0))
	{
		rotation_angle_rad = glm::radians(rotation_angle_deg);
	}

	if (ImGui::SliderFloat3("Scale", sclf, -2.0, 2.0))
	{
		setScale(glm::vec3(sclf[0], sclf[1], sclf[2]));
	}
}

std::vector<IDrawable*> WorldObject::getChildren() const
{
	return std::vector<IDrawable*>();
}

std::vector<IDrawable*> WorldObject::detachChildren()
{
	return std::vector<IDrawable*>();
}

void WorldObject::setRotation(glm::mat4 rot)
{
	rotation = rot;
	recalculateTransform();
}

void WorldObject::setScale(glm::vec3 scl)
{
	scale = scl;
}

void WorldObject::recalculateTransform()
{
	//we cannot do scaling this way, have to do those separately
	transform = glm::inverse(position * rotation);
}
