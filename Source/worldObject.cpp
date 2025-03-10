#include "header/worldObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

WorldObject::WorldObject()
	:position(glm::mat4(1.0f)),
	rotation(glm::mat4(1.0f)),
	scale(glm::vec3(1.0f))
{
	recalculateTransform();
}

void WorldObject::drawUI()
{
	if (ImGui::InputFloat3("Rotation", rotf))
	{
		float radiansX = glm::radians(rotf[0]);
		float radiansY = glm::radians(rotf[1]);
		float radiansZ = glm::radians(rotf[2]);

		glm::mat4 matX = glm::rotate(glm::mat4(1.0f), radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 matY = glm::rotate(glm::mat4(1.0f), radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 matZ = glm::rotate(glm::mat4(1.0f), radiansZ, glm::vec3(0.0f, 0.0f, 1.0f));

		setRotation(matX * matY * matZ);
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