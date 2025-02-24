#ifndef SDFBOX_H
#define SDFBOX_H

#include <glm/glm.hpp>

class SDFBox
{
public:
	SDFBox(glm::vec3 dimenions);

	void setPosition(glm::vec3 pos);
	void setRotation(glm::mat4 rot);
	void setScale(glm::vec3 scl);

	glm::vec2 sdf(glm::vec3 point) const;

private:
	float vmax(glm::vec3 v) const;
	void recalculateTransform();

	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;

	glm::mat4 transform;

	glm::vec3 dimensions;
};

#endif // !SDFBOX_H