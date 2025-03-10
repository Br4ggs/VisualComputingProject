#include "header/sdfBox.h"
#include "header/vecUtils.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include <iostream>

SDFBox::SDFBox(glm::vec3 dimensions)
	:WorldObject(),
	 dimensions(dimensions)
{
	color = glm::vec3(colf[0], colf[1], colf[2]);
}

void SDFBox::drawUI()
{
	if (ImGui::ColorEdit3("Color", colf))
	{
		color = glm::vec3(colf[0], colf[1], colf[2]);
	}

	WorldObject::drawUI();
}

char* SDFBox::getName() const
{
	return "box";
}

std::pair<float, glm::vec3> SDFBox::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	glm::vec3 d = glm::abs(point) - dimensions;
	float distance = glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0.0f)));
	return std::make_pair(distance * vmin(scale), color);
}