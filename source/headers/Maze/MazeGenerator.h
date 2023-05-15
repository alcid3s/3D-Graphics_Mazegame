#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
enum Type;

class CubeCreator;
class Texture;

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const float& x = 10.f, const float& y = 10.f);
	void DrawMaze();

	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);
private:
	int x,y;
	std::vector<std::vector<Type>> maze;

	CubeCreator* cubeCreator;
	std::vector<Texture*> mazeTextures;

	void PlaceBush(const float& x, const float& z);
	void PlaceFloor(const float& x, const float& z);
};