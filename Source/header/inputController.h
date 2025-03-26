#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "scene.h"
#include "GLFW/glfw3.h"

class InputController
{
public:
	InputController(GLFWwindow* window, Scene* scene);

	void processInput();

private:
	GLFWwindow* window;
	Scene* scene;

	bool firstMousePress = true;
	double firstXpos;
	double firstYpos;
	glm::vec3 firstCam;
	glm::vec3 firstLookAt;
};

#endif