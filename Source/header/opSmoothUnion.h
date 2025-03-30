#ifndef OPSMOOTHUNION_H
#define OPSMOOTHUNION_H

#include "iDrawable.h"

class OpSmoothUnion : public IDrawable
{
public:
	OpSmoothUnion(IDrawable* operant1, IDrawable* operant2);
	~OpSmoothUnion() override;

	void drawUI(bool& dirty) override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;
	float getSmoothingFactor();

	const CSGType getType() const override { return CSGOperation::OP_SMUN; };

private:
	float smoothing_factor = 0.5f;
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
