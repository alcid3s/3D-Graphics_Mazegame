#include "Maze/MazeGenerator.h"
#include "Tile.h"
#include "Texture.h"
#include "enumType.h"
#include "headers/components/Component.h"
#include "headers/components/CubeComponent.h"
// #include "headers/components/Component.h"
#include "gameobjects/Cube.h"
#include "gameobjects/Plane.h"
#include "modelLoader/ObjModel.h"
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

std::vector<std::vector<Tile*>> maze;

MazeGenerator::MazeGenerator() : x(0), y(0) {
	// added pointer of Texture to the vector.
	mazeTextures.push_back(new Texture("resource/textures/Floor4.png"));
	mazeTextures.push_back(new Texture("resource/textures/Bush_Texture4.png"));

	altar = new ObjModel("resource/models/altar/Altar.obj");
}

MazeGenerator::~MazeGenerator() {

}

void MazeGenerator::Generate(const int& sizeOfMazeX, const int& sizeOfMazeZ) {
	if (sizeOfMazeX < 5 || sizeOfMazeZ < 5) {
		return;
	}

	srand(time(NULL));

	// setup maze to traverse.
	SetupMaze(sizeOfMazeX, sizeOfMazeZ);

	// getting the starting tile.
	spawnTile = maze[(int)spawnPoint.z * -1][(int)spawnPoint.x * -1];
	spawnTile->type = Type::Floor;
	//spawnTile->setGameobject(PlaceFloor(spawnTile->GetPosition().x, spawnTile->GetPosition().z));
	std::vector<Tile*> visitedTiles;

	// create maze with DFS algorithm
	DepthFirstSearch(spawnTile, &visitedTiles);

	// create a random endpoint somewhere not close to the spawnPoint.
	endTile = visitedTiles.at(((visitedTiles.size() - 1) / 4) * 3);
	endTile->type = Type::Endpoint;
	endTile->setModel(altar);
	endPoint = endTile->GetPosition();

	// Fill empty spots with Walls.
	// FillMaze(sizeOfMazeX, sizeOfMazeZ);

	// set enemy spawnpoint (next to altar)
	SetEnemySpawnPoint(sizeOfMazeX, sizeOfMazeZ);
}

void MazeGenerator::DepthFirstSearch(Tile* tile, std::vector<Tile*>* visitedTiles) {
	// std::cout << "checking tile: (" << tile->GetPosition().x << "," << tile->GetPosition().z << "). ";
	tile->visited = true;

	std::vector<Tile*> neighbours = GetUnvisitedNeighbours(tile);

	while (!neighbours.empty()) {
		int random = rand() % neighbours.size();
		Tile* nextTile = neighbours[random];

		//nextTile->setGameobject(PlaceFloor(nextTile->GetPosition().x, nextTile->GetPosition().z));
		nextTile->type = Type::Floor;

		visitedTiles->push_back(tile);

		DepthFirstSearch(nextTile, visitedTiles);

		neighbours = GetUnvisitedNeighbours(tile);
	}

	if (visitedTiles->size() == amountOfTiles)
		return;

	Tile* backtrackTile = nullptr;
	for (int i = visitedTiles->size() - 1; i >= 0; i--) {
		Tile* tile = visitedTiles->at(i);
		std::vector<Tile*> neighbors = GetUnvisitedNeighbours(tile);

		if (!neighbors.empty()) {
			backtrackTile = tile;
			break;
		}
	}

	if (backtrackTile) {
		DepthFirstSearch(backtrackTile, visitedTiles);
	}
}

bool NextToFloor(Tile* tile) {
	int connections = 0;
	std::vector<Tile*> neighbours = GetNeighbours(tile);
	for (auto& tile : neighbours) {
		if (tile->type == Type::Floor) {
			connections++;
		}
	}
	// std::cout << "checking nextTile: (" << tile->GetPosition().x << "," << tile->GetPosition().z << ") has " << connections << ".\n";
	return connections > 1;
}

std::vector<Tile*> GetNeighbours(Tile* tile) {
	std::vector<Tile*> neighbours;
	Tile* neighbour;

	// get north neighbour
	//std::cout << "North: (" << tile->GetPosition().x << "," << tile->GetPosition().z - 1 << "), \n";
	neighbour = maze[tile->GetPosition().z - 1][tile->GetPosition().x];
	if (neighbour) {
		// std::cout << "north: (" << neighbour->GetPosition().x << "," << neighbour->GetPosition().z << "), ";
		neighbours.push_back(neighbour);
	}

	// get east neighbour
	//std::cout << "East: (" << tile->GetPosition().x + 1<< "," << tile->GetPosition().z << "), \n";
	neighbour = maze[tile->GetPosition().z][tile->GetPosition().x + 1];
	if (neighbour) {
		// std::cout << "east: (" << neighbour->GetPosition().x << "," << neighbour->GetPosition().z << "), ";
		neighbours.push_back(neighbour);
	}

	// get south neighbour
	//std::cout << "South: (" << tile->GetPosition().x << "," << tile->GetPosition().z + 1<< "), \n";
	neighbour = maze[tile->GetPosition().z + 1][tile->GetPosition().x];
	if (neighbour) {
		// std::cout << "south: (" << neighbour->GetPosition().x << "," << neighbour->GetPosition().z << "), \n";
		neighbours.push_back(neighbour);
	}

	// get west neighbour
	//std::cout << "West: (" << tile->GetPosition().x - 1 << "," << tile->GetPosition().z << "), \n";
	neighbour = maze[tile->GetPosition().z][tile->GetPosition().x - 1];
	if (neighbour) {
		// std::cout << "west: (" << neighbour->GetPosition().x << "," << neighbour->GetPosition().z << ")\n";
		neighbours.push_back(neighbour);
	}

	return neighbours;
}

std::vector<Tile*> GetUnvisitedNeighbours(Tile* tile) {
	std::vector<Tile*> neighbours = GetNeighbours(tile);

	/*std::cout << "NN: (" << neighbours[0]->GetPosition().x << "," << neighbours[0]->GetPosition().z << "), ";
	std::cout << "EN: (" << neighbours[1]->GetPosition().x << "," << neighbours[1]->GetPosition().z << "), ";
	std::cout << "SN: (" << neighbours[2]->GetPosition().x << "," << neighbours[2]->GetPosition().z << "), ";
	std::cout << "WN: (" << neighbours[3]->GetPosition().x << "," << neighbours[3]->GetPosition().z << ")\n";*/

	std::vector<Tile*> tilesToRemove;
	for (auto& tile : neighbours) {
		if (tile->visited || NextToFloor(tile))
			tilesToRemove.push_back(tile);
	}
	neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(), [&tilesToRemove](Tile* tile) {
		return std::find(tilesToRemove.begin(), tilesToRemove.end(), tile) != tilesToRemove.end();
		}), neighbours.end());

	return neighbours;
}

void MazeGenerator::DrawMaze() {
	for (auto& file : maze) {
		for (auto& tile : file) {
			if (tile)
				tile->draw();
		}
	}
}

void MazeGenerator::FillMaze(const int& sizeX, const int& sizeZ) {
	for (int z = 0; z < sizeZ; z++) {
		for (int x = 0; x < sizeX; x++)
		{
			Tile* tile = maze[z][x];
			if (tile->type == Type::Empty) {
				tile->type = Type::Wall;
				// tile->setGameobject(PlaceWall(x, z));
			}
		}
	}
}

void MazeGenerator::SetupMaze(const int& sizeOfMazeX, const int& sizeOfMazeZ) {
	spawnPoint = SetSpawnPoint(sizeOfMazeX, sizeOfMazeZ);
	std::cout << "Spawnpoint: (" << -spawnPoint.x << "," << -spawnPoint.z << ")\n";

	// set an empty maze with walls to work with.
	for (int z = 0; z <= sizeOfMazeZ; z++) {
		std::vector<Tile*> file;
		for (int x = 0; x <= sizeOfMazeX; x++) {

			// check if it's an edge. If so, place Wall.
			if (IsEdge(x, z, sizeOfMazeX, sizeOfMazeZ)) {
				file.push_back(new Tile(PlaceWall((float)x, (float)z), Type::Edge, glm::vec3(x, 0, z), true));
			}
			// not an edge
			else {
				amountOfTiles++;
				file.push_back(new Tile(PlaceEmptyGameobject(x, z), Type::Empty, glm::vec3(x, 0, z), false));
			}
		}
		maze.push_back(file);
	}
}

glm::vec3 MazeGenerator::SetSpawnPoint(const int& sizeX, const int& sizeZ) {
	std::vector<glm::vec3*> possibleSpawnPoints;
	for (int x = 0; x <= sizeX; x++) {
		for (int z = 0; z <= sizeZ; z++) {
			if (NextToEdge(x, z, sizeX, sizeZ)) {
				possibleSpawnPoints.push_back(new glm::vec3((float)x, 0.f, (float)z));
			}
		}
	}
	glm::vec3 sPoint = *possibleSpawnPoints.at(rand() % possibleSpawnPoints.size());
	sPoint.x *= -1;
	sPoint.z *= -1;
	return sPoint;
}

void MazeGenerator::SetEnemySpawnPoint(const int &sizeX, const int &sizeZ) {
	std::vector<Tile*> neighbours = GetNeighbours(endTile);

	for (auto& neighbour : neighbours) {
		if (neighbour->type == Type::Floor) {
			this->enemySpawnTile = neighbour;
			break;
		}
	}
}

bool MazeGenerator::NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ) {
	// if on north or south edge of maze.
	if (z == (sizeZ - 1) || z == 1) {
		if (x > 0 && x < sizeX) {
			return true;
		}
	}
	// if at east or west edge.
	else if (x == (sizeX - 1) || x == 1) {
		if (z > 0 && z < sizeZ) {
			return true;
		}
	}
	return false;
}

bool MazeGenerator::IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ) {
	return (x == sizeX || x == 0 || z == 0 || z == sizeZ);
}

CubeComponent* MazeGenerator::PlaceEmptyGameobject(const int& x, const int& z) {
	return new CubeComponent(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		nullptr
	);
}

CubeComponent* MazeGenerator::PlaceWall(const float& x, const float& z)
{
	return new CubeComponent(
		glm::vec3(1, 1, 1),
		glm::vec3(x, 0.f, z),
		mazeTextures[1]
	);
}

//Plane* MazeGenerator::PlaceFloor(const float& x, const float& z)
//{
//	return new Plane(
//		glm::vec3(1, 0, 1),
//		glm::vec3(x, -.5f, z),
//		mazeTextures[0]
//	);
//}

ObjModel* MazeGenerator::PlaceAltar()
{
	return altar;
}