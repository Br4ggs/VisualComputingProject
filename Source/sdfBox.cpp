#include "header/sdfBox.h"
#include <glm/gtc/matrix_transform.hpp>
#include "header/vecUtils.h"

SDFBox::SDFBox(glm::vec3 dimensions)
	:WorldObject(),
	 dimensions(dimensions)
{}

glm::vec2 SDFBox::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	glm::vec3 d = glm::abs(point) - dimensions;
	float distance = glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0.0f)));
	return glm::vec2(distance * vmin(scale), 1.0);
}