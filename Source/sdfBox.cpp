#include "header/sdfBox.h"
#include <glm/gtc/matrix_transform.hpp>
#include "header/vecUtils.h"

#include <iostream>

#include "imgui.h"

SDFBox::SDFBox(glm::vec3 dimensions)
	:WorldObject(),
	 dimensions(dimensions)
{}

void SDFBox::drawUI()
{
	if (ImGui::TreeNode("cube"))
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
			color = glm::vec3(colf[0], colf[1], colf[2]);
		}

	    ImGui::TreePop();
	}
}

//TODO: change return type to a float, vector pair
std::pair<float, glm::vec3> SDFBox::sdf(glm::vec3 point) const
{
	glm::vec4 pointHomog(point.x, point.y, point.z, 1.0f);
	point = transform * pointHomog;

	point = point / scale;

	glm::vec3 d = glm::abs(point) - dimensions;
	float distance = glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0.0f)));
	return std::make_pair(distance * vmin(scale), color);
}