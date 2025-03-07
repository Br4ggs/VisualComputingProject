#ifndef OPUNION_H
#define OPUNION_H

#include "iDrawable.h"

class OpUnion : public IDrawable
{
public:
	OpUnion(IDrawable* operant1, IDrawable* operant2);
	~OpUnion();

	//setters and getters?

	virtual void drawUI();
	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif