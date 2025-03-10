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
	point = point - scale * glm::round(point / scale);
	return operant->sdf(point);
}