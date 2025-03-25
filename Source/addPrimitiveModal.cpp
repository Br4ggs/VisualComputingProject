#include "header/addPrimitiveModal.h"

#include "header/sdfSphere.h"
#include "header/sdfBox.h"
#include "header/sdfCylinder.h"
#include "header/sdfPlane.h"
#include "header/OpenGLMarcher.h"

#include "imgui.h"

void AddPrimitiveModal::drawUI(Scene& scene) const
{
	ImGui::SetNextWindowSize(ImVec2(250, 150));

	if (ImGui::BeginPopupModal("Add primitive", NULL, ImGuiWindowFlags_None))
	{
        const char* primTypes[] = { "sphere", "box", "cylinder", "plane" };
        static int primSelected = 0;
        const char* primPreview = primTypes[primSelected];

		if (ImGui::BeginCombo("primitive", primPreview))
		{
			for (int n = 0; n < 4; n++)
			{
				const bool selected = (primSelected == n);
				if (ImGui::Selectable(primTypes[n], selected))
					primSelected = n;

				if (selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Add"))
		{
			IDrawable* prim = createPrimitive(primSelected);
			scene.addObject(prim);
			ImGui::CloseCurrentPopup();
			OpenGLMarcher::dirty = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

IDrawable* AddPrimitiveModal::createPrimitive(int prim) const
{
	switch (prim)
	{
	case 0: //sphere
		return new SDFSphere(1.0f);
	case 1: //box
		return new SDFBox(glm::vec3(1.0f, 1.0f, 1.0f));
	case 2: //cylinder
		return new SDFCylinder(1.0f, 1.0f);
	case 3: //plane
		return new SDFPlane(-0.5f);
		//return new SDFCylinder(1.0f, 1.0f);
	}
}
