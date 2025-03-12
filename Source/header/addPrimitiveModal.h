#ifndef ADDPRIMITIVEMODAL_H
#define ADDPRIMITIVEMODAL_H

#include "scene.h"
#include "iDrawable.h"

class AddPrimitiveModal
{
public:
	AddPrimitiveModal() = default;

	void drawUI(Scene& scene) const;

private:
	IDrawable* createPrimitive(int prim) const;
};

#endif
