#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <glm/glm.hpp>

class RayMarcher
{
public:
	RayMarcher(unsigned int width, unsigned int height);
	~RayMarcher();

	void render();
	unsigned char* getRenderData() const;

private:
	unsigned int width;
	unsigned int height;

	float FOV = 1.0;
	int maxSteps = 256;
	float maxDist = 500;
	float epsilon = 0.001;

	unsigned char* data = nullptr;

	glm::mat3 getCamera(glm::vec3 rayOrigin, glm::vec3 lookAt) const;
	
	glm::vec3 getMaterial(glm::vec3 point, float id) const;

	glm::vec3 getLight(glm::vec3 point, glm::vec3 rayDirection, glm::vec3 color) const;

	glm::vec3 getNormal(glm::vec3 point) const;

	glm::vec2 rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection) const;

	//TODO: move this stuff to a "scene" class
	glm::vec2 map(glm::vec3 point) const;

	float sdfCylinder(glm::vec3 point, float radius, float height) const;

	float sdfBox(glm::vec3 point, glm::vec3 dim) const;
	
	float sdfSphere(glm::vec3 point, float radius) const;

	float sdfPlane(glm::vec3 point, glm::vec3 normal, float distanceFromOrigin) const;

	glm::vec2 sdfUnion(glm::vec2 obj1, glm::vec2 obj2) const;

	glm::vec2 sdfIntersect(glm::vec2 obj1, glm::vec2 obj2) const;

	glm::vec2 sdfDifference(glm::vec2 obj1, glm::vec2 obj2) const;

	float vmax(glm::vec3 v) const;

	glm::vec3 repeat(glm::vec3 point, glm::vec3 scale) const;
};

#endif
