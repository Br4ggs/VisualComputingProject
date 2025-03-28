#ifndef OPINTERSECT_H

#include "iDrawable.h"

class OpIntersect : public IDrawable
{
public:
	OpIntersect(IDrawable* operant1, IDrawable* operant2);
	~OpIntersect() override;

	void drawUI(bool& dirty) override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

protected:
	CSGType type = CSGOperation::OP_INT;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
