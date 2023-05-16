#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Tile;
class Cube;
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
	int x,y;
	std::vector<std::vector<Tile*>> maze;
	std::vector<Texture*> mazeTextures;

	// private functions to setup the maze
private:
	bool IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);
	bool NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);

	glm::vec3 SetSpawnPoint(const int&, const int&);
	Tile* PlaceEmptyGameobject(const int& x, const int& z);
	Tile* PlaceWall(const float& x, const float& z, Type type, bool visited);
	Tile* PlaceFloor(const float& x, const float& z, bool visited);

	// functions to walk the maze and apply prims algorithm
private:
	std::vector<Tile*> GetUnvisitedNeighbours(Tile* tile);
};