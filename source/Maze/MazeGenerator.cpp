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

	// constants that will always be on this location no matter the size.
	const glm::vec2 leftFront = glm::vec2(0, 0);
	const glm::vec2 rightFront = glm::vec2(sizeOfMazeX, 0);
	const glm::vec2 leftBack = glm::vec2(0, sizeOfMazeZ);
	const glm::vec2 rightBack = glm::vec2(sizeOfMazeX, sizeOfMazeZ);

	spawnPoint = SetSpawnPoint(sizeOfMazeX, sizeOfMazeZ);

	for (int z = 0; z <= sizeOfMazeZ; z++) {
		std::vector<Tile*> file;
		for (int x = 0; x <= sizeOfMazeX; x++) {

			// check if it's an edge. If so, place Wall.
			if (IsEdge(x, z, sizeOfMazeX, sizeOfMazeZ)) {
				file.push_back(PlaceWall((float)x, (float)z));
			}
			// not an edge
			else {
				file.push_back(PlaceFloor((float)x, (float)z));
			}
		}
		maze.push_back(file);
	}

	//for (int i = 0; i < 1000; i += 10) {
	//	std::vector<Tile*> skybox;
	//	for (int j = 0; j < 1000; j += 10) {
	//		skybox.push_back(new Tile(new Plane(
	//			glm::vec3(10.f, 0.f, 10.f),
	//			glm::vec3(0.f, 0.f, 0.f),
	//			glm::vec3(j, 10.f, i),
	//			mazeTextures[2],
	//			1
	//		), Type::Skybox));
	//	}
	//	maze.push_back(skybox);
	// }
}

void MazeGenerator::DrawMaze() {
	for (auto file : maze) {
		for (auto tile : file) {
			tile->draw();
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

bool MazeGenerator::IsCorner(const int& x, const int& z, 
	const glm::vec2& lf, const glm::vec2& lb, 
	const glm::vec2& rf, const glm::vec2& rb) {

	glm::vec2 xy = glm::vec2(x, y);
	return xy == lf || xy == lb || xy == rf || xy == rb;
}

Tile* MazeGenerator::PlaceWall(const float &x, const float &z)
{
	return new Tile(new Cube(
		glm::vec3(1, 2, 1),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		mazeTextures[1],
		1
	), Type::Wall);
}

Tile* MazeGenerator::PlaceFloor(const float& x, const float& z)
{
	return new Tile(new Plane(
		glm::vec3(1, 0, 1),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(x, -.5f, z),
		mazeTextures[0],
		1
	), Type::Floor);
}
