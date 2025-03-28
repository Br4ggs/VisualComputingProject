#include "header/sdfSphere.h"
#include "header/vecUtils.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include <iostream>

SDFSphere::SDFSphere(float radius)
	:WorldObject(),
	 radius(radius)
{
	color = glm::vec3(colf[0], colf[1], colf[2]);
}

void SDFSphere::drawUI(bool& dirty)
{
	if (ImGui::ColorEdit3("Color", colf))
	{
		dirty = true;
		color = glm::vec3(colf[0], colf[1], colf[2]);
	}

	WorldObject::drawUI(dirty);
}

char* SDFSphere::getName() const
{
	return "sphere";
}

std::pair<float, glm::vec3> SDFSphere::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	float dist = glm::length(point) - radius;
	return std::make_pair(dist * vmin(scale), color);
}