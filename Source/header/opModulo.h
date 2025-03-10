#ifndef OPMODULO_H
#define OPMODULO_H

#include "iDrawable.h"

class OpModulo : public IDrawable
{
public:
	OpModulo(IDrawable* operant);
	~OpModulo();

	virtual void drawUI();
	virtual char* getName() const;
	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::vector<IDrawable*> detachChildren();
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	float sclf[3] = { 0.0f, 0.0f, 0.0f };

	IDrawable* operant;
};

#endif