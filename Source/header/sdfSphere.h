#ifndef SDFSPHERE_H
#define SDFSPHERE_H

#include "worldObject.h"
#include "types.h"
#include <glm/glm.hpp>

class SDFSphere : public WorldObject
{
public:
	SDFSphere(float radius);

	void drawUI(bool& dirty) override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

	const CSGType getType() const override { return CSGShape::SHAPE_SPHERE; };

private:
	float colf[3] = { 1.0f, 1.0f, 1.0f };

	float radius = 1.0f;
};

#endif
