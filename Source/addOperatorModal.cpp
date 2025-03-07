#include "header/addOperatorModal.h"
#include "header/opUnion.h"
#include "header/opIntersect.h"
#include "header/opDifference.h"

#include "imgui.h"

#include <iostream>

//TODO: make sure names are displayed in modal
//TODO: close button
//TODO: give preview of to-be-added object

void AddOperatorModal::drawUI(Scene& scene)
{
    if (ImGui::BeginPopupModal("Operator", NULL, ImGuiWindowFlags_None))
    {
        const char* opTypes[] = { "union", "intersect", "difference" };
        static int opSelected = 0;
        const char* opPreview = opTypes[opSelected];

        if (ImGui::BeginCombo("operator", opPreview))
        {
            for (int n = 0; n < 3; n++)
            {
                const bool selected = (opSelected == n);
                if (ImGui::Selectable(opTypes[n], opSelected))
                    opSelected = n;

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

            ImGui::BulletText("test");
            selectButton(obj);

            ImGui::PopID();
        }

        if (state == 2 && ImGui::Button("Add operator"))
        {
            IDrawable* op = createOperator(opSelected, operant1, operant2);

            scene.removeObject(operant1);
            scene.removeObject(operant2);
            scene.addObject(op);

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
            state = 1;
            break;
        case 1:
            operant2 = obj;
            state = 2;
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
    }
}