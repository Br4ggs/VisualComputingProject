#ifndef OPENGLMARCHER_H
#define OPENGLMARCHER_H

#include <glm/glm.hpp>
#include "scene.h"
#include "shaderProgram.h"

class OpenGLMarcher
{
public:
	OpenGLMarcher(unsigned int width,
							 unsigned int height,
							 Scene *scene,
							 ShaderProgram *shaderProgram);
	~OpenGLMarcher();

	void drawUI();
	void render(int width, int height);
	void linearize();

	static bool dirty;

private:

	Scene* currentScene;

	std::vector<LinearCSGTreeNode> linearScene;

	unsigned int width;
	unsigned int height;
	ShaderProgram *shaderProgram;
	Scene *scene;
	unsigned int uboID;
	unsigned int VAOID;

	float FOV = 1.0;
	int maxSteps = 256;
	float maxDist = 500;
	double fogCreep = -0.0008;
	float epsilon = 0.001;
	float colf[3] = { 0.5f, 0.8f, 0.9f };

	glm::vec3 backgroundColor = glm::vec3(colf[0], colf[1], colf[2]);
};

#endif
