#ifndef OPUNION_H
#define OPUNION_H

#include "iDrawable.h"

class OpUnion : public IDrawable
{
public:
	OpUnion(IDrawable* operant1, IDrawable* operant2);
	~OpUnion() override;

	CSGType getType() const { return CSGOperation::OP_UNI; };

	void drawUI(bool& dirty) override;
	char* getName() const override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
