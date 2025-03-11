#ifndef OPUNION_H
#define OPUNION_H

#include "iDrawable.h"

class OpUnion : public IDrawable
{
public:
	OpUnion(IDrawable* operant1, IDrawable* operant2);
	~OpUnion();

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