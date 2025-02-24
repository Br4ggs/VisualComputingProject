#ifndef SDFSPHERE_H
#define SDFSPHERE_H

#include <glm/glm.hpp>

class SDFSphere
{
public:
	SDFSphere(float radius);

	void setPosition(glm::vec3 pos);
	void setRotation(glm::mat4 rot);
	void setScale(glm::vec3 scl);

	glm::vec2 sdf(glm::vec3 point) const;

private:
	void recalculateTransform();

	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;

	glm::mat4 transform;

	float radius = 1.0f;
};

#endif // !SDFSPHERE_H
