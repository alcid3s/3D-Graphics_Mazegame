#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Texture.h"

struct Cube {
	Texture* texture;
	glm::vec3 transform;
	glm::vec3 translate;
};

class CubeCreator {
public:
	CubeCreator();
	void DrawCubes();
	void AddCube(const glm::vec3& transform, const glm::vec3& translate, Texture& texture);
	void AddCube(const glm::vec3& transform, const glm::vec3& translate);
	std::vector<Cube> getCubes();
private:
	void DrawCubePC(const glm::vec3& size);
	void DrawCubePT(const glm::vec3& size, Texture& texture);
};