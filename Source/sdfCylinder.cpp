#include "header/sdfCylinder.h"
#include "header/vecUtils.h"

#include "imgui.h"

SDFCylinder::SDFCylinder(float radius, float height)
	:WorldObject(),
	radius(radius),
	height(height)
{
	color = glm::vec3(colf[0], colf[1], colf[2]);
}

void SDFCylinder::drawUI(bool& dirty)
{
	if (ImGui::ColorEdit3("Color", colf))
	{
		dirty = true;
		color = glm::vec3(colf[0], colf[1], colf[2]);
	}

	WorldObject::drawUI(dirty);
}

char* SDFCylinder::getName() const
{
	return "cylinder";
}

std::pair<float, glm::vec3> SDFCylinder::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	float dist = glm::length(glm::vec2(point.x, point.z)) - radius;
	dist = glm::max(dist, abs(point.y) - height);
	return std::make_pair(dist * vmin(scale), color);
}