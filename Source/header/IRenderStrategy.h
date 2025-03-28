#ifndef IRENDERSTRATEGY_H
#define IRENDERSTRATEGY_H

#include "types.h"
#include "GLFW/glfw3.h"

class IRenderStrategy 
{
public:
	IRenderStrategy(RenderStrategy::Type type);
	virtual ~IRenderStrategy() = default;

	virtual void render(Scene *scene, GLFWwindow* window) = 0;
	virtual void switchedStrategy() = 0;
	virtual void setup() = 0;
	virtual void drawUI(bool &dirty) = 0;
	virtual void dirtyUpdate(Scene *scene, GLFWwindow *window) = 0;

	const char* getName() { return RenderStrategy::toString(type); };
	const RenderStrategy::Type getType() { return type; };

private:
	RenderStrategy::Type type;
};

#endif
