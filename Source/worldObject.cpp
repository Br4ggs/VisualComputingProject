#include "header/worldObject.h"
#include <glm/gtc/matrix_transform.hpp>

WorldObject::WorldObject()
	:position(glm::mat4(1.0f)),
	rotation(glm::mat4(1.0f)),
	scale(glm::vec3(1.0f))
{
	recalculateTransform();
}

std::vector<IDrawable*> WorldObject::getChildren() const
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