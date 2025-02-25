#include "header/sdfSphere.h"
#include "header/vecUtils.h"
#include <glm/gtc/matrix_transform.hpp>

SDFSphere::SDFSphere(float radius)
	:WorldObject(),
	 radius(radius)
{}

glm::vec2 SDFSphere::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	float dist = glm::length(point) - radius;
	return glm::vec2(dist * vmin(scale), 1.0);
}