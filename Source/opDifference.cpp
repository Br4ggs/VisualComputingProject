#include "header/opDifference.h"

#include "imgui.h"

OpDifference::OpDifference(IDrawable* operant1, IDrawable* operant2)
	:operant1(operant1), operant2(operant2)
{}

OpDifference::~OpDifference()
{
	delete operant1;
	delete operant2;
}

void OpDifference::drawUI(bool& dirty)
{
	if (ImGui::Button("swap operants"))
	{
		dirty = true;
		IDrawable* temp = operant1;
		operant1 = operant2;
		operant2 = temp;
	}

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

char* OpDifference::getName() const
{
	return "difference";
}

std::vector<IDrawable*> OpDifference::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::vector<IDrawable*> OpDifference::detachChildren()
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	operant1 = nullptr;
	operant2 = nullptr;

	return operants;
}

std::pair<float, glm::vec3> OpDifference::sdf(glm::vec3 point) const
{
	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);

	obj2.first *= -1;

	if (obj1.first > obj2.first)
		return obj1;
	else
		return obj2;
}