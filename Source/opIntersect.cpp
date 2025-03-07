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
	if (ImGui::TreeNode("intersect"))
	{
		ImGui::PushID(0);
		operant1->drawUI();
		ImGui::PopID();

		ImGui::PushID(1);
		operant2->drawUI();
		ImGui::PopID();

		ImGui::TreePop();
	}
}

std::vector<IDrawable*> OpIntersect::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant1);
	operants.push_back(operant2);
	return operants;
};

std::pair<float, glm::vec3> OpIntersect::sdf(glm::vec3 point) const
{
	std::pair<float, glm::vec3> obj1 = operant1->sdf(point);
	std::pair<float, glm::vec3> obj2 = operant2->sdf(point);

	return (obj1.first > obj2.first) ? obj1 : obj2;
}