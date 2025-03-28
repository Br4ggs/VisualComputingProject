#include "header/opSmoothUnion.h"
#include "imgui.h"

OpSmoothUnion::OpSmoothUnion(IDrawable* operant1, IDrawable* operant2)
	:operant1(operant1), operant2(operant2)
{}

OpSmoothUnion::~OpSmoothUnion()
{
	if (operant1 != nullptr)
		delete operant1;
	if (operant2 != nullptr)
		delete operant2;
}

float OpSmoothUnion::getSmoothingFactor()
{
	return smoothing_factor;
}

void OpSmoothUnion::drawUI(bool& dirty)
{
	ImGui::PushID(0);

	if (ImGui::SliderFloat("smoothing factor", &smoothing_factor, 0.01, 3.0)) {
		dirty = true;
	}

	if (ImGui::TreeNode(operant1->getName()))
	{
		operant1->drawUI(dirty);
		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PushID(1);
	if (ImGui::TreeNode(operant2->getName()))
	{
		operant2->drawUI(dirty);
		ImGui::TreePop();
	}
	ImGui::PopID();
}

char* OpSmoothUnion::getName() const
{
	return "smooth union";
}

std::vector<IDrawable*> OpSmoothUnion::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::vector<IDrawable*> OpSmoothUnion::detachChildren()
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	operant1 = nullptr;
	operant2 = nullptr;

	return operants;
}

std::pair<float, glm::vec3> OpSmoothUnion::sdf(glm::vec3 point) const
{
	//input guards

	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);

	float h = glm::clamp(0.5 + 0.5 * (obj2.first - obj1.first) / smoothing_factor, 0.0, 1.0);
	float mixed = glm::mix(obj2.first, obj1.first, h);

	float distVal =  mixed - smoothing_factor * h * (1.0 - h);

	return (obj1.first < obj2.first) ? std::pair(distVal, obj1.second) : std::pair(distVal, obj2.second);
}
