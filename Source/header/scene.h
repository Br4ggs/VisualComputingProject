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

	void drawUI(bool& dirty);
	std::pair<float, glm::vec3> map(glm::vec3 point) const;

	glm::vec3 getCamPos() const;
	glm::vec3 getLookAt() const;

	glm::vec3 getLightPos() const;
	glm::vec3 getSpecColor() const;

	std::vector<IDrawable*> getObjects();
	void addObject(IDrawable* obj);
	void removeObject(IDrawable* obj);

	float ambientColorf[3] = { 0.3f, 0.3f, 0.3f };

	int fov = 60;
	float specular_strength = 0.5;
	float shininess = 64.0;

private:
	float camPosf[3] = { 0.0f, 0.0f, -3.0f };
	float lookAtf[3] = { 0.0f, 0.0f, 0.0f };

	float lightPosf[3] = { 30.0f, 40.0f, 30.0f };
	float specColorf[3] = { 0.5f, 0.5f, 0.5f };

	glm::vec3 specularColor = glm::vec3(0.5f);

	//list of Idrawables allocated on the heap
	std::vector<IDrawable*> objects;

	void objectNodeWithDelete(IDrawable* obj, bool& dirty);

	void deleteAsParent(IDrawable* obj);

	std::pair<float, glm::vec3> sdfUnion(std::pair<float, glm::vec3> obj1, std::pair<float, glm::vec3> obj2) const;
};

#endif
