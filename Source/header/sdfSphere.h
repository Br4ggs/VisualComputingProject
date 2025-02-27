#ifndef SDFSPHERE_H
#define SDFSPHERE_H

#include "worldObject.h"
#include <glm/glm.hpp>

class SDFSphere : public WorldObject
{
public:
	SDFSphere(float radius);

	virtual void drawUI();
	virtual glm::vec2 sdf(glm::vec3 point) const;

private:
	float radius = 1.0f;
};

#endif // !SDFSPHERE_H
