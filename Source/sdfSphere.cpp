#include "header/sdfSphere.h"
#include "header/vecUtils.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include <iostream>

SDFSphere::SDFSphere(float radius)
	:WorldObject(),
	 radius(radius)
{}

void SDFSphere::drawUI()
{
	//position
	if (ImGui::InputFloat3("Position", posf))
	{
		setPosition(glm::vec3(posf[0], posf[1], posf[2]));
	}

	//TODO: rotation?

	//scale
	if (ImGui::InputFloat3("Scale", sclf))
	{
		setScale(glm::vec3(sclf[0], sclf[1], sclf[2]));
	}

	//color
	if (ImGui::ColorEdit3("Color", colf))
	{
		color = glm::vec3(colf[0], colf[1], colf[2]);
	}
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