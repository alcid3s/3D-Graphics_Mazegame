#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Texture.h"

struct Cube {
	glm::vec3 transform;
	glm::vec3 translate;
};

class CubeCreator {
public:
	CubeCreator();
	void AddCube(const glm::vec3& transform, const glm::vec3& translate, const Texture& texture);
	void AddCube(const glm::vec3& transform, const glm::vec3& translate);
	std::vector<Cube> getCubes();
private:
	void DrawCubePC(const glm::vec3& size);
	void DrawCubePT(const glm::vec3& size, const Texture& texture);
};