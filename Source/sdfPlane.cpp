#include "header/sdfPlane.h"
#include "header/vecUtils.h"

#include "imgui.h"

//TODO: fix weird rendering issue where a ball and a plane in a scene causes the
//		ball to not be rendered properly?

SDFPlane::SDFPlane(float height)
	:WorldObject(),
	height(height)
{}

void SDFPlane::drawUI(bool& dirty)
{
	WorldObject::drawUI(dirty);
}

char* SDFPlane::getName() const
{
	return "plane";
}

std::pair<float, glm::vec3> SDFPlane::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	float dist = glm::dot(point, normal) + height;
	return std::make_pair(dist * vmin(scale), getColor(point));
}

glm::vec3 SDFPlane::getColor(glm::vec3 point) const
{
	//try solid red color?
	return glm::vec3(0.2f + 0.4f * glm::mod(floor(point.x) + floor(point.z), 2.0f));
}