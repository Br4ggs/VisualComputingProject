#ifndef OPUNION_H
#define OPUNION_H

#include "iDrawable.h"

class OpUnion : public IDrawable
{
public:
	OpUnion(IDrawable* operant1, IDrawable* operant2);
	~OpUnion() override;

	void drawUI(bool& dirty) override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

protected:
	CSGType type = CSGOperation::OP_UNI;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
