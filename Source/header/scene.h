#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include "iDrawable.h"
#include "sdfBox.h";
#include "sdfSphere.h"

class Scene
{
public:
	Scene();
	~Scene();

	void drawUI();
	glm::vec2 map(glm::vec3 point) const;

	glm::vec3 getCamPos() const;
	glm::vec3 getLookAt() const;

	glm::vec3 getLightPos() const;
	glm::vec3 getSpecColor() const;

private:
	float camPosf[3] = { 0.0f, 0.0f, -3.0f };
	float lookAtf[3] = { 0.0f, 0.0f, 0.0f };

	float lightPosf[3] = { 30.0f, 40.0f, 30.0f };
	float specColorf[3] = { 0.5f, 0.5f, 0.5f };

	glm::vec3 specularColor = glm::vec3(0.5f);

	//how to store the objects in a scene
	//option 1: store on heap and store pointers in vector
	//-kinda dangerous

	//list of Idrawable
	std::vector<IDrawable*> objects;

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
