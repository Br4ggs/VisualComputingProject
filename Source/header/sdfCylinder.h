#ifndef SDFCYLINDER_H
#define SDFCYLINDER_H

#include "worldObject.h"

class SDFCylinder : public WorldObject
{
public:
	SDFCylinder(float radius, float height);

	virtual void drawUI();
	virtual char* getName() const;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

	CSGType getType() const { return CSGShape::SHAPE_CYL; };

private:
	float colf[3] = { 1.0f, 1.0f, 1.0f };

	float radius = 1.0f;
	float height = 1.0f;
};

#endif
