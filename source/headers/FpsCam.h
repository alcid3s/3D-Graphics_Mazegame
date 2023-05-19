#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class FpsCam {
public:
	FpsCam(GLFWwindow*);

	glm::mat4 getMatrix();
	void update(GLFWwindow*, float deltaTime);
	void setEndpoint(glm::vec3);

	inline float GetFov() const { return fov; }
	bool endPointReached;

	glm::vec3* position;
private:
	float fov;
	bool shiftPressed;
	glm::vec3 endPoint;
	glm::vec2 rotation = glm::vec2(0, 180);
	void move(float angle, float fac, float deltaTime);
	void moveCam(GLFWwindow* window, const float& speed, float deltaTime);
	void changeFov(float deltaTime);
	bool isAtEndpoint(float tolerance);
};