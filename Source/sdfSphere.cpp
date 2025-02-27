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
	if (ImGui::TreeNode("sphere"))
	{
		//position
		static float posf[3] = { 0.0f, 0.0f, 0.0f };
		if (ImGui::InputFloat3("Position", posf))
		{
			setPosition(glm::vec3(posf[0], posf[1], posf[2]));
		}

		//TODO: rotation?

		//scale
		static float sclf[3] = { 1.0f, 1.0f, 1.0f };
		if (ImGui::InputFloat3("Scale", sclf))
		{
			setScale(glm::vec3(sclf[0], sclf[1], sclf[2]));
		}

		//color
		static float colf[3] = { 1.0f, 1.0f, 1.0f };
		if (ImGui::ColorEdit3("Color", colf))
		{
			//TODO
		}

		ImGui::TreePop();
	}
}

glm::vec2 SDFSphere::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	float dist = glm::length(point) - radius;
	return glm::vec2(dist * vmin(scale), 1.0);
}