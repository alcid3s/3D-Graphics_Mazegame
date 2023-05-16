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

	bool IsCorner(const int& x, const int& z, const glm::vec2& lf, const glm::vec2& lb, const glm::vec2& rf, const glm::vec2& rb);
	bool IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);
	bool NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);

	glm::vec3 SetSpawnPoint(const int&, const int&);

	Tile* PlaceWall(const float& x, const float& z);
	Tile* PlaceFloor(const float& x, const float& z);
};