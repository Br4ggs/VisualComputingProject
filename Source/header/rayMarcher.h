#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <glm/glm.hpp>
#include "scene.h"

class RayMarcher
{
public:
	RayMarcher(unsigned int width, unsigned int height);
	~RayMarcher();

	void drawUI();
	void render(Scene* scene);
	unsigned char* getRenderData() const;

private:

	Scene* scene;

	unsigned int width;
	unsigned int height;

	float FOV = 1.0;
	int maxSteps = 256;
	float maxDist = 500;
	double fogCreep = -0.0008;
	float epsilon = 0.001;
	float minEpsilon = 0.00001f; //?
	float colf[3] = { 0.5f, 0.8f, 0.9f };

	glm::vec3 backgroundColor = glm::vec3(colf[0], colf[1], colf[2]);

	unsigned char* data = nullptr;

	glm::mat3 getCamera(glm::vec3 rayOrigin, glm::vec3 lookAt) const;

	glm::vec3 getLight(glm::vec3 point, glm::vec3 rayDirection, glm::vec3 color) const;

	glm::vec3 getNormal(glm::vec3 point) const;

	std::pair<float, glm::vec3> rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection) const;
};

#endif
