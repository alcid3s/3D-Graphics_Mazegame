#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class FpsCam {
public:
	FpsCam(GLFWwindow*);

	glm::mat4 getMatrix();
	void update(GLFWwindow*);
	glm::vec3* position;
private:
	glm::vec2 rotation = glm::vec2(0, 0);
	void move(float angle, float fac);
	void moveCam(GLFWwindow* window, const float& speed);
};