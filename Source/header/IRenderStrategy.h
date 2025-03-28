#ifndef IRENDERSTRATEGY_H
#define IRENDERSTRATEGY_H

#include "types.h"
#include "GLFW/glfw3.h"
#include "scene.h"

class IRenderStrategy 
{
public:
	IRenderStrategy(RenderStrategyType type);
	virtual ~IRenderStrategy() = default;

	virtual void render(Scene *scene, GLFWwindow* window) = 0;
	virtual void switchedStrategy() = 0;
	virtual void setup() = 0;
	virtual void drawUI(bool &dirty) = 0;
	virtual void dirtyUpdate() = 0;

	const char* getName() { return strategyToString(type); };

private:
	RenderStrategyType type;
};

#endif
