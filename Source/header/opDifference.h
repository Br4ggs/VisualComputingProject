#ifndef OPDIFFERENCE_H
#define OPDIFFERENCE_H

#include "iDrawable.h"

class OpDifference : public IDrawable
{
public:
	OpDifference(IDrawable* operant1, IDrawable* operant2);
	~OpDifference() override;

	void drawUI(bool& dirty) override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

	const CSGType getType() const override { return CSGOperation::OP_DIFF; };

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
