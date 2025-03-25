#include "header/worldObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "OpenGLMarcher.h"

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
	if (ImGui::SliderFloat3("Position", posf, -5.0, 5.0))
	{
		OpenGLMarcher::dirty = true;
	}

	if (ImGui::SliderFloat3("Rotation", rotf, -1.0, 1.0))
	{
		OpenGLMarcher::dirty = true;
	}

	if (ImGui::SliderFloat("Rotation Angle", &rotation_angle_deg, 0.0, 360.0))
	{
		rotation_angle_rad = glm::radians(rotation_angle_deg);
		OpenGLMarcher::dirty = true;
	}

	if (ImGui::SliderFloat3("Scale", sclf, -2.0, 2.0))
	{
		OpenGLMarcher::dirty = true;
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
