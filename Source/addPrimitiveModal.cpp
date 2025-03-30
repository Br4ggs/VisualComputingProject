#include "header/addPrimitiveModal.h"

#include "header/sdfSphere.h"
#include "header/sdfBox.h"
#include <iostream>
#include "header/sdfCylinder.h"
#include "header/sdfPlane.h"

#include "imgui.h"


void AddPrimitiveModal::drawUI(Scene& scene, bool& dirty) const
{
  ImGui::SetNextWindowSize(ImVec2(250, 150));

  if (ImGui::BeginPopupModal("Add primitive", NULL, ImGuiWindowFlags_None))
  {
    static int selectedId = 0;
    static CSGType selectedShape = parseCSGType(shapeNames[selectedId]).value();
    const char* primPreview = shapeNames[selectedId];

    if (ImGui::BeginCombo("primitive", primPreview))
    {
      for (int n = 0; n < CSGShape::NO_SHAPE; n++)
      {
        const bool selected = (selectedId == n);
        if (ImGui::Selectable(shapeNames[n], selected)) {
          selectedId = n;
          selectedShape = parseCSGType(shapeNames[n]).value();
          std::cout << "selected shape: " << shapeNames[n] << std::endl;
          std::cout << "parsed shape: " << selectedShape << std::endl;
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
      std::cout << "adding selected shape: " << selectedShape << std::endl;
      IDrawable* prim = AddPrimitiveModal::createPrimitive(std::get<CSGShape>(selectedShape));
      std::cout << "created primitve: " << prim->getType() << std::endl;
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

  std::cout << "creating primitive " << shape << std::endl; // this line prints the correct value
  switch (shape)
  {
  case SHAPE_SPHERE: //sphere
    return new SDFSphere(1.0f);
  case SHAPE_BOX: //box
    std::cout << "alskdfjaklsdfjalskdjflaskdjflaksdjf" << std::endl;
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
