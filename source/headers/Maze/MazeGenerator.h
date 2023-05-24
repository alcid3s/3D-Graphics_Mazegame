#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class CubeComponent;
class PlaneComponent;
class ObjModel;

class GameObject;

class Tile;
class Texture;

enum Type;

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	std::list<std::shared_ptr<GameObject>> Generate(const int& sizeOfMazeX = 10, const int& sizeOfMazeZ = 10);
	void DrawMaze();

	// place where enemy will spawn.
	Tile* enemySpawnTile;

	// tile where player will begin
	Tile* spawnTile;
	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);

	// place where altar will stand
	Tile* endTile;
	glm::vec3 endPoint = glm::vec3(2, 0, 2);
	

	// private variables
private:
	int x, y;
	int amountOfTiles = 0;
	std::vector<Texture*> mazeTextures;

	ObjModel* altar;
	// private functions to setup the maze
private:
	bool IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);
	bool NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ);

	void SetupMaze(const int& sizeOfMazeX, const int& sizeOfMazeZ, std::list<std::shared_ptr<GameObject>>*);

	glm::vec3 SetSpawnPoint(const int&, const int&);
	void SetEnemySpawnPoint(const int& sizeX, const int& sizeZ);
	CubeComponent* PlaceEmptyGameobject(const int& x, const int& z);
	CubeComponent* PlaceWall(const float& x, const float& z);
	PlaneComponent* PlaceFloor(const float& x, const float& z);
	ObjModel* PlaceAltar();

	// functions to walk the maze and apply DFS algorithm
private:
	void DepthFirstSearch(Tile* tile, std::vector<Tile*>* visitedTiles);
	void FillMaze(const int& sizeX, const int& sizeZ);
};

// out of class because they're needed in FpsCam too.
std::vector<Tile*> GetUnvisitedNeighbours(Tile* tile);
std::vector<Tile*> GetNeighbours(Tile* tile);
bool NextToFloor(Tile* tile);
