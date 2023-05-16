#include "Maze/MazeGenerator.h"
#include "Tile.h"
#include "Texture.h"
#include "enumType.h"
#include "gameobjects/Gameobject.h"
#include "gameobjects/Cube.h"
#include "gameobjects/Plane.h"
#include <vector>
#include <random>

#include <iostream>

MazeGenerator::MazeGenerator() : x(0), y(0) {
	// added pointer of Texture to the vector.
	mazeTextures.push_back(new Texture("resource/textures/Floor4.png"));
	mazeTextures.push_back(new Texture("resource/textures/Bush_Texture4.png"));
	mazeTextures.push_back(new Texture("resource/textures/Skybox1.png"));
}

MazeGenerator::~MazeGenerator() {

}

void MazeGenerator::Generate(const int& sizeOfMazeX, const int& sizeOfMazeZ) {
	srand(time(NULL));

	spawnPoint = SetSpawnPoint(sizeOfMazeX, sizeOfMazeZ);

	// set an empty maze with walls to work with.
	for (int z = 0; z <= sizeOfMazeZ; z++) {
		std::vector<Tile*> file;
		for (int x = 0; x <= sizeOfMazeX; x++) {

			// check if it's an edge. If so, place Wall.
			if (IsEdge(x, z, sizeOfMazeX, sizeOfMazeZ)) {
				file.push_back(PlaceWall((float)x, (float)z, Type::Edge, true));
			}
			// not an edge
			else {
				file.push_back(PlaceEmptyGameobject(x, z));
			}
		}
		maze.push_back(file);
	}
}

std::vector<Tile*> MazeGenerator::GetUnvisitedNeighbours(Tile * tile) {
	std::vector<Tile*> neighbours;

	return neighbours;
}

void MazeGenerator::DrawMaze() {
	for (auto file : maze) {
		for (auto tile : file) {
			if (tile) {
				tile->draw();
			}
		}
	}
}

glm::vec3 MazeGenerator::SetSpawnPoint(const int &sizeX, const int &sizeZ) {
	const int edgesOfMaze = (sizeX - 4) * (sizeZ - 4);

	// create a number between the edges of the maze and 4. (standard 36 if maze is 10x10).
	int iterator = (rand() % edgesOfMaze) + 4;

	std::cout << "iterator: " << iterator << "\n";
	for (int x = 0; x <= sizeX; x++)
		for (int z = 0; z <= sizeZ; z++) {
			if (NextToEdge(x, z, sizeX, sizeZ))
				iterator--;
			if (iterator == 0) {
				std::cout << "spawnpos: " << x << ',' << z << "\n";
				return glm::vec3((float)x * -1, 0, (float)z * -1);
			}
		}
}

bool MazeGenerator::NextToEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ) {
	return (x == (sizeX - 1) && x != 0|| x == 1 || z == (sizeZ - 1) && z != 0 || z == 1);
		
}

bool MazeGenerator::IsEdge(const int& x, const int& z, const int& sizeX, const int& sizeZ) {
	return (x == sizeX || x == 0 || z == 0 || z == sizeZ);
}

Tile* MazeGenerator::PlaceEmptyGameobject(const int& x, const int& z) {
	return new Tile(new Cube(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		nullptr,
		1
	), Type::Empty, glm::vec3(x, -5.f, z), false);
}

Tile* MazeGenerator::PlaceWall(const float &x, const float &z, Type type, bool visited)
{
	return new Tile(new Cube(
		glm::vec3(1, 2, 1),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		mazeTextures[1],
		1
	), type, glm::vec3(x, -.5f, z), visited);
}

Tile* MazeGenerator::PlaceFloor(const float& x, const float& z, bool visited)
{
	return new Tile(new Plane(
		glm::vec3(1, 0, 1),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		mazeTextures[0],
		1
	), Type::Floor, glm::vec3(x, -.5f, z), visited);
}
