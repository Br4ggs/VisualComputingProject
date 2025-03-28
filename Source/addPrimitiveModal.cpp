#include "header/addPrimitiveModal.h"

#include "header/sdfSphere.h"
#include "header/sdfBox.h";
#include <iostream>
#include "header/sdfCylinder.h"
#include "header/sdfPlane.h"

#include "imgui.h"

void AddPrimitiveModal::drawUI(Scene& scene, bool& dirty) const
{
  ImGui::SetNextWindowSize(ImVec2(250, 150));

  if (ImGui::BeginPopupModal("Add primitive", NULL, ImGuiWindowFlags_None))
  {
    CSGType selectedShape;
    static int selectedId = 0;
    const char* primPreview = shapeNames[selectedId];

    if (ImGui::BeginCombo("primitive", primPreview))
    {
      for (int n = 0; n < CSGShape::NO_SHAPE; n++)
      {
        const bool selected = (selectedId == n);
        if (ImGui::Selectable(shapeNames[n], selected)) {
          selectedShape = parseCSGType(shapeNames[n]).value();
          selectedId = n;
        }

        if (selected) {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }

    if (ImGui::Button("Add"))
    {
      dirty = true;
      IDrawable* prim = createPrimitive(std::get<CSGShape>(selectedShape));
      scene.addObject(prim);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Close"))
    {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

IDrawable* AddPrimitiveModal::createPrimitive(CSGShape shape) const
{

  std::cout << shape;
  switch (shape)
  {
  case SHAPE_SPHERE: //sphere
    return new SDFSphere(1.0f);
  case SHAPE_BOX: //box
    return new SDFBox(glm::vec3(1.0f, 1.0f, 1.0f));
  case SHAPE_CYL: //cylinder
    return new SDFCylinder(1.0f, 1.0f);
  case SHAPE_PLANE: //plane
    return new SDFPlane(-0.5f);
  case NO_SHAPE:
    throw std::logic_error("not a real shape");
    break;
  }
}
