#include "header/inputController.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

InputController::InputController(GLFWwindow* window, Scene* scene)
	:window(window),
	 scene(scene)
{}

void InputController::processInput()
{
	//TODO: deltatime

	int stateW = glfwGetKey(window, GLFW_KEY_W); //forward
	int stateA = glfwGetKey(window, GLFW_KEY_A); //left
	int stateS = glfwGetKey(window, GLFW_KEY_S); //backward
	int stateD = glfwGetKey(window, GLFW_KEY_D); //right
	int stateQ = glfwGetKey(window, GLFW_KEY_Q); //up
	int stateE = glfwGetKey(window, GLFW_KEY_E); //down

	int stateM1 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stateM2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (stateM2 == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << xpos << std::endl;

		if (firstMousePress)
		{
			firstXpos = xpos;
			firstYpos = ypos;
			firstCam = scene->getCamPos();
			firstLookAt = scene->getLookAt();
			firstMousePress = false;
		}

		double deltaX = firstXpos - xpos;
		double deltaY = firstYpos - ypos;

		//glm::vec3 cam = scene->getCamPos();
		//glm::vec3 lookAt = scene->getLookAt();

		glm::vec4 camHomog(firstCam.x, firstCam.y, firstCam.z, 1.0f);
		glm::vec4 lookAtHomog(firstLookAt.x, firstLookAt.y, firstLookAt.z, 0.0f);

		glm::vec3 camForward = glm::normalize(firstLookAt - firstCam);
		glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
		glm::vec3 camUp = glm::cross(camForward, camRight);

		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), (float)deltaY * 0.01f, camRight);
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), (float)deltaX * -0.01f, glm::vec3(0, 1, 0));

		//handle offset using translation matrices?
		camHomog -= lookAtHomog;
		camHomog = camHomog * rotX * rotY;
		camHomog += lookAtHomog;

		glm::vec3 cam = camHomog;
		scene->setCamPos(cam);
	}
	else if (stateM2 == GLFW_RELEASE)
		firstMousePress = true;

	glm::vec3 offset(0.0f);

	if (stateW == GLFW_PRESS)
	{
		offset.z += 0.01f;
	}

	if (stateA == GLFW_PRESS)
	{
		offset.x += 0.01f;
	}

	if (stateS == GLFW_PRESS)
	{
		offset.z -= 0.01f;
	}

	if (stateD == GLFW_PRESS)
	{
		offset.x -= 0.01f;
	}

	if (stateQ == GLFW_PRESS)
	{
		offset.y += 0.01f;
	}

	if (stateE == GLFW_PRESS)
	{
		offset.y -= 0.01f;
	}

	scene->setCamPos(scene->getCamPos() + offset);
	scene->setLookAt(scene->getLookAt() + offset);
}