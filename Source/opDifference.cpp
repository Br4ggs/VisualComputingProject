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

void OpDifference::drawUI()
{
	if (ImGui::TreeNode("difference"))
	{
		if (ImGui::Button("swap operants"))
		{
			IDrawable* temp = operant1;
			operant1 = operant2;
			operant2 = temp;
		}

		ImGui::PushID(0);
		operant1->drawUI();
		ImGui::PopID();

		ImGui::PushID(1);
		operant2->drawUI();
		ImGui::PopID();

		ImGui::TreePop();
	}
}

std::vector<IDrawable*> OpDifference::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::pair<float, glm::vec3> OpDifference::sdf(glm::vec3 point) const
{
	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);

	if (obj1.first > -obj2.first)
	{
		return obj1;
	}
	else
	{
		obj2.first *= -1;
		return obj2;
	}
}