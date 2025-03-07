#ifndef OPDIFFERENCE_H
#define OPDIFFERENCE_H

#include "iDrawable.h"

class OpDifference : public IDrawable
{
public:
	OpDifference(IDrawable* operant1, IDrawable* operant2);
	~OpDifference();

	virtual void drawUI();
	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif