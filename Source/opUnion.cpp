#include "header/opUnion.h"

#include "imgui.h"

OpUnion::OpUnion(IDrawable* operant1, IDrawable* operant2)
	:operant1(operant1), operant2(operant2)
{}

OpUnion::~OpUnion()
{
	if (operant1 != nullptr)
		delete operant1;
	if (operant2 != nullptr)
		delete operant2;
}

void OpUnion::drawUI(bool& dirty)
{
	ImGui::PushID(0);
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

std::vector<IDrawable*> OpUnion::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::vector<IDrawable*> OpUnion::detachChildren()
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	operant1 = nullptr;
	operant2 = nullptr;

	return operants;
}

std::pair<float, glm::vec3> OpUnion::sdf(glm::vec3 point) const
{
	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);
	return (obj1.first < obj2.first) ? obj1 : obj2;
}
