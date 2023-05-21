#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Texture.h"
#include "enumType.h"

//struct Cube {
//	Texture* texture;
//	Type type;
//	glm::vec3 transform;
//	glm::vec3 translate;
//};

class CubeCreator {
public:
	CubeCreator(const int& textureMultiplier = 1, const int& floorTextureMulti = 25);
	~CubeCreator();
	void DrawCubes();
	void AddCube(const glm::vec3& transform, const glm::vec3& translate, Texture* texture, Type type);
	void AddCube(const glm::vec3& transform, const glm::vec3& translate, Type type);
	std::vector<Cube> getCubes();
private:
	int textureMultiplier;
	int floorTextureMultiplier;
	void DrawCubePC(const glm::vec3& size);
	void DrawCubePT(const glm::vec3& size, Texture& texture, const int& texMultiplier);
};