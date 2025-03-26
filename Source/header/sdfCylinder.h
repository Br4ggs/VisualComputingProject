#ifndef SDFCYLINDER_H
#define SDFCYLINDER_H

#include "worldObject.h"
#include "types.h"

class SDFCylinder : public WorldObject
{
public:
	SDFCylinder(float radius, float height);

	CSGType getType() const { return CSGShape::SHAPE_CYL; };

	void drawUI(bool& dirty) override;
	char* getName() const override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

private:
	float colf[3] = { 1.0f, 1.0f, 1.0f };

	float radius = 1.0f;
	float height = 1.0f;
};

#endif
