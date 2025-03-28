#ifndef OPSMOOTHUNION_H
#define OPSMOOTHUNION_H

#include "iDrawable.h"

class OpSmoothUnion : public IDrawable
{
public:
	OpSmoothUnion(IDrawable* operant1, IDrawable* operant2);
	~OpSmoothUnion() override;

	CSGType getType() const { return CSGOperation::OP_SMUN; };

	void drawUI(bool& dirty) override;
	char* getName() const override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;
	float getSmoothingFactor();

private:
	float smoothing_factor;
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
