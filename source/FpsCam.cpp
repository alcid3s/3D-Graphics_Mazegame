#include "FpsCam.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
FpsCam::FpsCam(GLFWwindow* window) : fov(80.f), shiftPressed(false){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

glm::mat4 FpsCam::getMatrix() {
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, *position);
	return ret;
}

void FpsCam::move(float angle, float fac, float deltaTime) {
	position->x += ((float)cos(rotation.y + glm::radians(angle)) * -fac) * deltaTime;
	position->z += ((float)sin(rotation.y + glm::radians(angle)) * -fac) * deltaTime;
}

void FpsCam::update(GLFWwindow* window, float deltaTime) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	static double lastX = x;
	static double lastY = y;

	float tempX = rotation.x - (float)(lastY - y) / 100.f;

	if (tempX < 0.5f && tempX > -0.5f)
		rotation.x = tempX;

	rotation.y -= (float)(lastX - x) / 100.f;

	lastX = x;
	lastY = y;

	moveCam(window, .75f, deltaTime);

	if (shiftPressed) {
		if (fov < 100.f) {
			fov -= 50.f * deltaTime;
		}
	}
	else {
		if (fov > 80.f) {
			fov += 50.f * deltaTime;
		}
	}
}

void FpsCam::moveCam(GLFWwindow* window, const float& speed, float deltaTime) {
	float mult = 1;

	// multiplier for running
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		mult *= 3.f;
		shiftPressed = true;
	}
	else
		shiftPressed = false;
		

	// left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move(0, speed, deltaTime);
	}

	// right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move(180, speed, deltaTime);
	}

	// forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move(90, speed * mult, deltaTime);
	}

	// backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move(-90, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position->y -= speed * 2.f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position->y += speed * 2.f;
	}
}