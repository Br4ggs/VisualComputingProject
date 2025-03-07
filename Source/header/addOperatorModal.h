#ifndef ADDOPERATORMODAL_H
#define ADDOPERATORMODAL_H

#include "scene.h"
#include "iDrawable.h"

class AddOperatorModal
{
public:
	AddOperatorModal() = default;

	void drawUI(Scene& scene);

private:
	void selectButton(IDrawable* obj);
	IDrawable* createOperator(int op, IDrawable* op1, IDrawable* op2) const;

	int state = 0;
	IDrawable* operant1 = nullptr;
	IDrawable* operant2 = nullptr;
};

#endif