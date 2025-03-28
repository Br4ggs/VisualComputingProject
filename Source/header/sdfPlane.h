#ifndef SDFPLANE_H
#define SDFPLANE_H

#include "worldObject.h"
#include "types.h"

class SDFPlane : public WorldObject
{
public:
	SDFPlane(float height);

	void drawUI(bool& dirty) override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

protected:
	CSGType type = CSGShape::SHAPE_PLANE;

private:
	float height = 0.0f;
	glm::vec3 normal = glm::vec3(0, 1, 0);

	glm::vec3 getColor(glm::vec3 point) const;
};

#endif
