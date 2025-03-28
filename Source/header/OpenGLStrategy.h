#ifndef OPENGLSTRATEGY_H
#define OPENGLSTRATEGY_H

#include <vector>
#include "types.h"
#include "shaderProgram.h"
#include "IRenderStrategy.h"

class OpenGLStrategy : public IRenderStrategy
{
public:
	OpenGLStrategy(RenderStrategy::Type type);
	~OpenGLStrategy();

	void render(Scene *scene, GLFWwindow* window);
	void switchedStrategy();
	void setup();
	void drawUI(bool &dirty);
	void dirtyUpdate(Scene *scene, GLFWwindow *window);

private:
	std::vector<LinearCSGTreeNode> linearScene{};
	ShaderProgram shaderProgram;
	GLint shaderProgramInt;

	unsigned int uboID;
	unsigned int VAOID;

	float FOV = 1.0;
	int maxSteps = 100;
	float maxDist = 50;
	float epsilon = 0.001;
	float minEpsilon = 0.000001f;
	float colf[3] = { 0.5f, 0.8f, 0.9f };

};

#endif
