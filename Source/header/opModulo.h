#ifndef OPMODULO_H
#define OPMODULO_H

#include "iDrawable.h"

class OpModulo : public IDrawable
{
public:
	OpModulo(IDrawable* operant);
	~OpModulo() override;

	void drawUI(bool& dirty) override;
	char* getName() const override;
	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

private:
	float sclf[3] = { 0.0f, 0.0f, 0.0f };
	bool axisb[3] = { true, true, true };

	IDrawable* operant;
};

#endif