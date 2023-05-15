#include "Maze/MazeGenerator.h"
#include "Tile.h"
#include "Texture.h"
#include "enumType.h"
#include "gameobjects/Gameobject.h"
#include "gameobjects/Cube.h"
#include "gameobjects/Plane.h"
#include <vector>
#include <random>

MazeGenerator::MazeGenerator() {
	// added pointer of Texture to the vector.
	mazeTextures.push_back(new Texture("resource/textures/test1.png"));
	mazeTextures.push_back(new Texture("resource/textures/Bush_Texture4.png"));
}

MazeGenerator::~MazeGenerator() {

}

void MazeGenerator::Generate(const int& sizeOfMazeX, const int& sizeOfMazeZ) {
	srand(time(NULL));

	// constants that will always be on this location no matter the size.
	const glm::vec2 leftFront = glm::vec2(((sizeOfMazeX * -1) / 2) + .25f, ((sizeOfMazeZ * -1) / 2) + .25f);
	const glm::vec2 rightFront = glm::vec2((sizeOfMazeX / 2) - .25f, ((sizeOfMazeZ * -1) / 2) + .25f);
	const glm::vec2 leftBack = glm::vec2(((sizeOfMazeX * -1) / 2) + .25f, (sizeOfMazeZ / 2) - .25f);
	const glm::vec2 rightBack = glm::vec2((sizeOfMazeX / 2) - .25f, (sizeOfMazeZ / 2) - .25f);

	spawnPoint = glm::vec3(-1, 0, -1);

	for (int y = 0; y < sizeOfMazeZ; y++) {
		std::vector<Tile*> file;
		for (int x = 0; x < sizeOfMazeZ; x++) {

			file.push_back(new Tile(new Plane(
				glm::vec3(1.f, .0f, 1.f),
				glm::vec3(0.f, 0.f, 0.f),
				glm::vec3(x, -.5f, y),
				mazeTextures[0],
				1
			), Type::Floor));
		}

		maze.push_back(file);
	}
}

void MazeGenerator::DrawMaze() {
	for (auto file : maze) {
		for (auto tile : file) {
			tile->draw();
		}
	}
}