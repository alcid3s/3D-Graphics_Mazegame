#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Cube {
	glm::vec3 transform;
	glm::vec3 translate;
};

class CubeCreator {
public:
	CubeCreator();
	void AddCube(const glm::vec3& transform, const glm::vec3& translate);
	std::vector<Cube> getCubes();
private:
	void DrawCube(const glm::vec3& size);
};