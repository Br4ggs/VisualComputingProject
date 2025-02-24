#include "header/sdfSphere.h"
#include <glm/gtc/matrix_transform.hpp>

SDFSphere::SDFSphere(float radius)
	:position(glm::mat4(1.0f)),
	rotation(glm::mat4(1.0f)),
	scale(glm::mat4(1.0f)),
	radius(radius)
{
	recalculateTransform();
}

void SDFSphere::setPosition(glm::vec3 pos)
{
	position = glm::translate(glm::mat4(1.0f), pos);
	recalculateTransform();
}

void SDFSphere::setRotation(glm::mat4 rot)
{
	rotation = rot;
	recalculateTransform();
}

void SDFSphere::setScale(glm::vec3 scl)
{
	scale = glm::scale(glm::mat4(1.0f), scl);
	recalculateTransform();
}

glm::vec2 SDFSphere::sdf(glm::vec3 point) const
{
	//glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	//point = transform * pointHomog;

	float dist = glm::length(point) - radius;
	return glm::vec2(dist, 1.0);
}

void SDFSphere::recalculateTransform()
{
	transform = glm::inverse(position * rotation * scale);
}