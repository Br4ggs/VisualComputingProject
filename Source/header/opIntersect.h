#ifndef OPINTERSECT_H

#include "iDrawable.h"

class OpIntersect : public IDrawable
{
public:
	OpIntersect(IDrawable* operant1, IDrawable* operant2);
	~OpIntersect();

	virtual void drawUI();
	virtual char* getName() const;
	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	IDrawable* operant1;
	IDrawable* operant2;
};

#endif // !OPINTERSECT_H
