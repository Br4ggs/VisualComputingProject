#include "glm/ext/matrix_transform.hpp"
#include "types.h"
#include "scene.h"
#include "header/inputController.h"
#include "imgui.h"


InputController::InputController(GLFWwindow* window, Scene* scene)
	:window(window),
	 scene(scene)
{}

void InputController::drawUI()
{
	//control explanation
	ImGui::Text("Use W/S keys to move forward/backward");
	ImGui::Text("Use A/D keys to move left/right");
	ImGui::Text("Use Q/E keys to move up/down");
	ImGui::Text("Use the scroll wheel to zoom in/out");
	ImGui::Text("Press and hold right mouse button to rotate \ncamera around current lookat point");

	//reset orientation
	if (ImGui::Button("Reset camera orientation"))
	{
		scene->setLookAt(glm::vec3(0.0f));
		scene->setCamPos(glm::vec3(0.0f, 0.0f, -3.0f));
	}

	//set scroll multiplier
	ImGui::InputFloat("scroll factor", &camZoomScalar);

	//set wasd speed multiplier
	ImGui::InputFloat("movement factor", &camMovementScalar);
}

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

	// mouse-based rotation
	if (stateM2 == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

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

		glm::vec4 camHomog(firstCam.x, firstCam.y, firstCam.z, 1.0f);
		glm::vec4 lookAtHomog(firstLookAt.x, firstLookAt.y, firstLookAt.z, 1.0f);

		glm::vec3 camForward = glm::normalize(firstLookAt - firstCam);
		glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
		glm::vec3 camUp = glm::cross(camForward, camRight);

		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), (float)deltaY * 0.01f, camRight);
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), (float)deltaX * 0.01f, glm::vec3(0, 1, 0));

		//handle offset using translation matrices?
		camHomog -= lookAtHomog;
		camHomog = camHomog * rotX * rotY;
		camHomog += lookAtHomog;

		glm::vec3 cam = camHomog;
		scene->setCamPos(cam);
	}
	else if (stateM2 == GLFW_RELEASE)
		firstMousePress = true;

	//disallow position moving when we are currently rotating
	if (stateM2 != GLFW_PRESS)
	{
		//moving the lookat and camera position
		glm::vec3 offset(0.0f);

		glm::vec3 cam = scene->getCamPos();
		glm::vec3 lookAt = scene->getLookAt();

		glm::vec3 camForward = glm::normalize(lookAt - cam);
		glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
		glm::vec3 camUp = glm::cross(camForward, camRight);

		if (stateW == GLFW_PRESS) offset += camForward * camMovementScalar;
		if (stateA == GLFW_PRESS) offset -= camRight * camMovementScalar;
		if (stateS == GLFW_PRESS) offset -= camForward * camMovementScalar;
		if (stateD == GLFW_PRESS) offset += camRight * camMovementScalar;
		if (stateQ == GLFW_PRESS) offset += camUp * camMovementScalar;
		if (stateE == GLFW_PRESS) offset -= camUp * camMovementScalar;

		scene->setCamPos(scene->getCamPos() + offset);
		scene->setLookAt(scene->getLookAt() + offset);
	}
}


void InputController::processScrollEvent(double xoffset, double yoffset)
{
	int stateM2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	//if we are currently rotating the camera we need to modify the original base position of the camera
	if (stateM2 == GLFW_PRESS && !firstMousePress)
	{
		glm::vec3 camForward = glm::normalize(firstLookAt - firstCam);
		firstCam += camForward * (float)yoffset * camZoomScalar;
	}
	//otherwise, we can manipulate the current camera position directly
	else
	{
		glm::vec3 cam = scene->getCamPos();
		glm::vec3 lookAt = scene->getLookAt();

		glm::vec3 camForward = glm::normalize(lookAt - cam);

		cam += camForward * (float)yoffset * camZoomScalar;
		scene->setCamPos(cam);
	}
}
