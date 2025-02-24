#include "header/sdfBox.h"
#include <glm/gtc/matrix_transform.hpp>

SDFBox::SDFBox(glm::vec3 dimensions)
	:position(glm::mat4(1.0f)),
	 rotation(glm::mat4(1.0f)),
	 scale(glm::mat4(1.0f)),
	 dimensions(dimensions)
{
	recalculateTransform();
}

void SDFBox::setPosition(glm::vec3 pos)
{
	position = glm::translate(glm::mat4(1.0f), pos);
	recalculateTransform();
}

void SDFBox::setRotation(glm::mat4 rot)
{
	rotation = rot;
	recalculateTransform();
}

void SDFBox::setScale(glm::vec3 scl)
{
	scale = glm::scale(glm::mat4(1.0f), scl);
	recalculateTransform();
}

glm::vec2 SDFBox::sdf(glm::vec3 point) const
{
	glm::vec3 scaler(0.75f, 0.15f, 0.5f);

	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scaler;

	glm::vec3 d = glm::abs(point) - dimensions;
	float distance = glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0.0f)));
	return glm::vec2(distance * glm::min(scaler.x, glm::min(scaler.y, scaler.z)), 1.0);
}

float SDFBox::vmax(glm::vec3 v) const
{
	return glm::max(glm::max(v.x, v.y), v.z);
}

void SDFBox::recalculateTransform()
{
	//we cannot do scaling this way, have to do those separately
	transform = glm::inverse(position * rotation);
}