#include "header/opIntersect.h"

#include "imgui.h"

OpIntersect::OpIntersect(IDrawable* operant1, IDrawable* operant2)
	:operant1(operant1), operant2(operant2)
{}

OpIntersect::~OpIntersect()
{
	delete operant1;
	delete operant2;
}

void OpIntersect::drawUI()
{
	ImGui::PushID(0);
	if (ImGui::TreeNode(operant1->getName()))
	{
		operant1->drawUI();
		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PushID(1);
	if (ImGui::TreeNode(operant2->getName()))
	{
		operant2->drawUI();
		ImGui::TreePop();
	}
	ImGui::PopID();
}

char* OpIntersect::getName() const
{
	return "intersect";
}

std::vector<IDrawable*> OpIntersect::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::vector<IDrawable*> OpIntersect::detachChildren()
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	operant1 = nullptr;
	operant2 = nullptr;

	return operants;
}

std::pair<float, glm::vec3> OpIntersect::sdf(glm::vec3 point) const
{
	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);

	return (obj1.first > obj2.first) ? obj1 : obj2;
}