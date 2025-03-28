#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "GLFW/glfw3.h"
#include "glm/ext/vector_float3.hpp"

class Scene;

class InputController
{
public:
	InputController(GLFWwindow* window, Scene* scene);

	void drawUI();
	void processInput();
	void processScrollEvent(double xoffset, double yoffset);

	static void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
		InputController* input = static_cast<InputController*>(glfwGetWindowUserPointer(window));
		if (input) {
			input->processScrollEvent(xoffset, yoffset);
		}
	}

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
