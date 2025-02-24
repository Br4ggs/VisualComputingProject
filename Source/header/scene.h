#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include "sdfBox.h";
#include "sdfSphere.h"

class Scene
{
public:
	Scene();

	glm::vec2 map(glm::vec3 point) const;

private:
	SDFBox box = SDFBox(glm::vec3(1.0f, 1.0f, 1.0f));
	SDFSphere sphere = SDFSphere(1.0f);


	float sdfCylinder(glm::vec3 point, float radius, float height) const;

	float sdfSphere(glm::vec3 point, float radius) const;

	float sdfPlane(glm::vec3 point, glm::vec3 normal, float distanceFromOrigin) const;

	glm::vec2 sdfUnion(glm::vec2 obj1, glm::vec2 obj2) const;

	glm::vec2 sdfIntersect(glm::vec2 obj1, glm::vec2 obj2) const;

	glm::vec2 sdfDifference(glm::vec2 obj1, glm::vec2 obj2) const;

	glm::vec3 repeat(glm::vec3 point, glm::vec3 scale) const;
};

#endif // !SCENE_H
