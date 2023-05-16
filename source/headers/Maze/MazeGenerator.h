#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Tile;
class Cube;
class Texture;

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const int& sizeOfMazeX = 10, const int& sizeOfMazeZ = 10);
	void DrawMaze();

	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);
private:
	int x,y;

	std::vector<std::vector<Tile*>> maze;

	std::vector<Texture*> mazeTextures;

	Tile* PlaceWall(const float& x, const float& z);
	Tile* PlaceBush(const float& x, const float& z);
};