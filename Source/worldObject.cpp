#include "header/worldObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

WorldObject::WorldObject()
{
	setPosition(glm::vec3(posf[0], posf[1], posf[2]));

	float radiansX = glm::radians(rotf[0]);
	float radiansY = glm::radians(rotf[1]);
	float radiansZ = glm::radians(rotf[2]);

	glm::mat4 matX = glm::rotate(glm::mat4(1.0f), radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matY = glm::rotate(glm::mat4(1.0f), radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 matZ = glm::rotate(glm::mat4(1.0f), radiansZ, glm::vec3(0.0f, 0.0f, 1.0f));

	setRotation(matX * matY * matZ);

	setScale(glm::vec3(sclf[0], sclf[1], sclf[2]));
}

void WorldObject::drawUI(bool& dirty)
{
	if (ImGui::InputFloat3("Position", posf))
	{
		dirty = true;
		setPosition(glm::vec3(posf[0], posf[1], posf[2]));
	}

	if (ImGui::InputFloat3("Rotation", rotf))
	{
		dirty = true;
		float radiansX = glm::radians(rotf[0]);
		float radiansY = glm::radians(rotf[1]);
		float radiansZ = glm::radians(rotf[2]);

		glm::mat4 matX = glm::rotate(glm::mat4(1.0f), radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 matY = glm::rotate(glm::mat4(1.0f), radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 matZ = glm::rotate(glm::mat4(1.0f), radiansZ, glm::vec3(0.0f, 0.0f, 1.0f));

		setRotation(matX * matY * matZ);
	}

	if (ImGui::InputFloat3("Scale", sclf))
	{
		dirty = true;
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

void WorldObject::setPosition(glm::vec3 pos)
{
	position = glm::translate(glm::mat4(1.0f), pos);
	recalculateTransform();
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