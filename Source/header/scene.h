#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include "iDrawable.h"

class Scene
{
public:
	Scene();
	~Scene();

	void drawUI();
	std::pair<float, glm::vec3> map(glm::vec3 point) const;

	glm::vec3 getCamPos() const;
	glm::vec3 getLookAt() const;

	glm::vec3 getLightPos() const;
	glm::vec3 getSpecColor() const;

	std::vector<IDrawable*> getObjects();
	void addObject(IDrawable* obj);
	void removeObject(IDrawable* obj);

private:
	float camPosf[3] = { 0.0f, 0.0f, -3.0f };
	float lookAtf[3] = { 0.0f, 0.0f, 0.0f };

	float lightPosf[3] = { 30.0f, 40.0f, 30.0f };
	float specColorf[3] = { 0.5f, 0.5f, 0.5f };

	glm::vec3 specularColor = glm::vec3(0.5f);

	//list of Idrawables allocated on the heap
	std::vector<IDrawable*> objects;

	void objectNodeWithDelete(IDrawable* obj);

	void deleteAsParent(IDrawable* obj);

	float sdfCylinder(glm::vec3 point, float radius, float height) const;

	float sdfPlane(glm::vec3 point, glm::vec3 normal, float distanceFromOrigin) const;

	std::pair<float, glm::vec3> sdfUnion(std::pair<float, glm::vec3> obj1, std::pair<float, glm::vec3> obj2) const;

	glm::vec3 repeat(glm::vec3 point, glm::vec3 scale) const;
};

#endif
