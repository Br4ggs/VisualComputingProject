#include "header/addOperatorModal.h"
#include "header/opUnion.h"
#include "header/opIntersect.h"
#include "header/opDifference.h"
#include "header/opModulo.h"

#include "imgui.h"

void AddOperatorModal::drawUI(Scene& scene, bool& dirty)
{
    ImGui::SetNextWindowSize(ImVec2(350, 300));

    if (ImGui::BeginPopupModal("Add operator", NULL, ImGuiWindowFlags_None))
    {
        const char* opTypes[] = { "union", "intersect", "difference", "modulo" };
        const char* opPreview = opTypes[selectedOperator];

        if (ImGui::BeginCombo("operator", opPreview))
        {
            for (int n = 0; n < 4; n++)
            {
                const bool selected = (selectedOperator == n);
                if (ImGui::Selectable(opTypes[n], selectedOperator))
                    selectedOperator = n;

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        switch (state)
        {
        case 0:
            ImGui::Text("Select first operant");
            break;
        case 1:
            ImGui::Text("Select second operant");
            break;
        }

        //list of current objects in the scene
        std::vector<IDrawable*> objects = scene.getObjects();
        for (int i = 0; i < objects.size(); i++)
        {
            IDrawable* obj = objects[i];

            if (obj == operant1 || obj == operant2 || state == 2) continue;

            ImGui::PushID(i);
            ImGui::BulletText(obj->getName());
            selectButton(obj);
            ImGui::PopID();
        }

        if (state == 2 && ImGui::Button("Add operator"))
        {
            dirty = true;
            IDrawable* op = createOperator(selectedOperator, operant1, operant2);

            scene.removeObject(operant1);
            if (selectedOperator != 3) //modulo only has one operant
                scene.removeObject(operant2); //keep in mind, otherwise will cause memleak

            scene.addObject(op);

            state = 0;
            operant1 = nullptr;
            operant2 = nullptr;

            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Close"))
        {
            state = 0;
            operant1 = nullptr;
            operant2 = nullptr;

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void AddOperatorModal::selectButton(IDrawable* obj)
{
    ImGui::SameLine();

    if (ImGui::SmallButton("select"))
    {
        switch (state)
        {
        case 0:
            operant1 = obj;
            if (selectedOperator == 3) //modulo
                state = 2;
            else
                state = 1;
            break;
        case 1:
            operant2 = obj;
            state = 2;
            break;
        }
    }
}

IDrawable* AddOperatorModal::createOperator(int op, IDrawable* op1, IDrawable* op2) const
{
    switch (op)
    {
    case 0: //union
        return new OpUnion(op1, op2);
    case 1: //intersect
        return new OpIntersect(op1, op2);
    case 2: //difference
        return new OpDifference(op1, op2);
    case 3: //modulo
        return new OpModulo(op1);
    }
}