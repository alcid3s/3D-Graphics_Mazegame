#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class FpsCam {
public:
	FpsCam(GLFWwindow*);

	glm::mat4 getMatrix();
	void update(GLFWwindow*, float deltaTime);
	inline float GetFov() const { return fov; }


	glm::vec3* position;
private:
	float fov;
	bool shiftPressed;
	glm::vec2 rotation = glm::vec2(0, 180);
	void move(float angle, float fac, float deltaTime);
	void moveCam(GLFWwindow* window, const float& speed, float deltaTime);
};