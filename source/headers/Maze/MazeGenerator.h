#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Cube;
class Plane;

class Tile;
class Texture;

enum Type;

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const int& sizeOfMazeX = 10, const int& sizeOfMazeZ = 10);
	void DrawMaze();

	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);

	// private variables
private:
	int x, y;
	int amountOfTiles = 0;
	std::vector<std::vector<Tile*>> maze;
	std::vector<Texture*> mazeTextures;

	// private functions to setup the maze
private:
	bool IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);
	bool NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);

	void SetupMaze(const int& sizeOfMazeX, const int& sizeOfMazeZ);

	glm::vec3 SetSpawnPoint(const int&, const int&);
	Cube* PlaceEmptyGameobject(const int& x, const int& z);
	Cube* PlaceWall(const float& x, const float& z);
	Plane* PlaceFloor(const float& x, const float& z);
	Plane* PlaceEndPoint(const float& x, const float& z);

	// functions to walk the maze and apply prims algorithm
private:
	void DepthFirstSearch(Tile* tile, std::vector<Tile*>* visitedTiles);
	std::vector<Tile*> GetUnvisitedNeighbours(Tile* tile);
	bool NextToFloor(Tile* tile);
	std::vector<Tile*> GetNeighbours(Tile* tile);
};