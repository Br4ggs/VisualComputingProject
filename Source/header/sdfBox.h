#ifndef SDFBOX_H
#define SDFBOX_H

#include "worldObject.h"
#include <glm/glm.hpp>

class SDFBox : public WorldObject
{
public:
	SDFBox(glm::vec3 dimenions);

	void drawUI(bool& dirty) override;
	char* getName() const override;
	std::pair<float, glm::vec3> sdf(glm::vec3 point) const override;

private:
	float colf[3] = { 1.0f, 1.0f, 1.0f };

	glm::vec3 dimensions;
};

#endif