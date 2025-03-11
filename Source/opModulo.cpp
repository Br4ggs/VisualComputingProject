#include "header/opModulo.h"

#include "imgui.h"

OpModulo::OpModulo(IDrawable* operant)
	:operant(operant)
{}

OpModulo::~OpModulo()
{
	if (operant != nullptr)
		delete operant;
}

void OpModulo::drawUI()
{
	ImGui::InputFloat3("scale", sclf);

	if (ImGui::TreeNode(operant->getName()))
	{
		operant->drawUI();
		ImGui::TreePop();
	}
}

char* OpModulo::getName() const
{
	return "modulo";
}

std::vector<IDrawable*> OpModulo::getChildren() const
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant);
	return operants;
}

std::vector<IDrawable*> OpModulo::detachChildren()
{
	std::vector<IDrawable*> operants;
	operants.push_back(operant);
	operant = nullptr;

	return operants;
}

std::pair<float, glm::vec3> OpModulo::sdf(glm::vec3 point) const
{
	glm::vec3 scale(sclf[0], sclf[1], sclf[2]);

	if (sclf[0] > 0)
		point.x = point.x - scale.x * glm::round(point.x / scale.x);
	if (sclf[1] > 0)
		point.y = point.y - scale.y * glm::round(point.y / scale.y);
	if (sclf[2] > 0)
		point.z = point.z - scale.z * glm::round(point.z / scale.z);

	return operant->sdf(point);
}