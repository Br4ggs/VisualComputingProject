#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "scene.h"
#include "GLFW/glfw3.h"

class InputController
{
public:
	InputController(GLFWwindow* window, Scene* scene);

	void drawUI();
	void processInput();
	void processScrollEvent(double xoffset, double yoffset);

private:
	GLFWwindow* window;
	Scene* scene;

	float camMovementScalar = 0.1f;
	float camZoomScalar = 0.1f;

	bool firstMousePress = true;
	double firstXpos;
	double firstYpos;
	glm::vec3 firstCam;
	glm::vec3 firstLookAt;
};

#endif