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

MazeGenerator::MazeGenerator() {
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

	spawnPoint = glm::vec3(-1, 0, -1);

	bool flag = false;
	for (int z = 0; z <= sizeOfMazeZ; z++) {
		std::vector<Tile*> file;
		for (int x = 0; x <= sizeOfMazeX; x++) {
			// check if it's an edge.
			if (z == sizeOfMazeZ || z == 0 || x == 0 || x == sizeOfMazeX) {
				file.push_back(PlaceWall(x, z));
			// not an edge
			} else {
				file.push_back(PlaceFloor(x, z));
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
