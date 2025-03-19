#ifndef OPDIFFERENCE_H
#define OPDIFFERENCE_H

#include "iDrawable.h"

class OpDifference : public IDrawable
{
public:
	OpDifference(IDrawable* operant1, IDrawable* operant2);
	~OpDifference();

	CSGType getType() const { return CSGOperation::OP_DIFF; };

	virtual void drawUI();
	virtual char* getName() const;
	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::vector<IDrawable*> detachChildren();
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif
