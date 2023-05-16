#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Tile;
class Cube;
class Texture;

struct Cell {
	Tile* tile;
	bool visited;
};

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const int& sizeOfMazeX = 10, const int& sizeOfMazeZ = 10);
	void DrawMaze();

	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);

	// private variables
private:
	int x,y;
	std::vector<std::vector<Cell*>> maze;
	std::vector<Texture*> mazeTextures;

	// private functions to setup the maze
private:
	bool IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);
	bool NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);

	glm::vec3 SetSpawnPoint(const int&, const int&);
	Tile* PlaceWall(const float& x, const float& z);
	Tile* PlaceFloor(const float& x, const float& z);

	// functions to walk the maze and apply prims algorithm
private:
	void WalkMaze();
	std::vector<Cell*> GetUnvisitedNeighbors(Cell* cell);
};