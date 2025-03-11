#ifndef SDFSPHERE_H
#define SDFSPHERE_H

#include "worldObject.h"
#include <glm/glm.hpp>

class SDFSphere : public WorldObject
{
public:
	SDFSphere(float radius);

	virtual void drawUI();
	virtual char* getName() const;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const;

private:
	float colf[3] = { 1.0f, 1.0f, 1.0f };

	float radius = 1.0f;
};

#endif
