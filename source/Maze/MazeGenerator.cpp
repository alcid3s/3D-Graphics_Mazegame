#include "Maze/MazeGenerator.h"
#include "CubeCreator.h"
#include "Texture.h"
#include "enumType.h"
#include "Tile.h"
#include <vector>
#include <random>

MazeGenerator::MazeGenerator() {
	cubeCreator = new CubeCreator();

	// added pointer of Texture to the vector.
	mazeTextures.push_back(new Texture("resource/textures/Floor4.png"));
	mazeTextures.push_back(new Texture("resource/textures/Bush_Texture4.png"));
}

MazeGenerator::~MazeGenerator() {

}

void MazeGenerator::Generate(const float& x, const float& z) {
	srand(time(NULL));

	// constants that will always be on this location no matter the size.
	const glm::vec2 leftFront = glm::vec2(((x * -1) / 2) + .25f, ((z * -1) / 2) + .25f);
	const glm::vec2 rightFront = glm::vec2((x / 2) - .25f, ((z * -1) / 2) + .25f);
	const glm::vec2 leftBack = glm::vec2(((x * -1) / 2) + .25f, (z / 2) - .25f);
	const glm::vec2 rightBack = glm::vec2((x / 2) - .25f, (z / 2) - .25f);

	// placing floor.
	PlaceFloor((float)x, (float)z);

	spawnPoint = glm::vec3(rand() % (int)(x / 2) - 2, 0, rand() % (int)(z / 2) - 2);

	std::vector<std::vector<Tile>> maze;

	for (float posY = leftFront.y; posY <= rightBack.y; posY += .5f) {
		for (float posX = leftFront.x; posX <= rightBack.x; posX += .5f) {

			// draw edges
			if (posY == leftFront.y || posY == rightBack.y || posX == leftFront.x || posX == rightBack.x) {
				PlaceBush(posX, posY);
			}
		}
	}
}

void MazeGenerator::DrawMaze() {
	cubeCreator->DrawCubes();
}

void MazeGenerator::PlaceBush(const float& x, const float& z) {
	cubeCreator->AddCube(
		glm::vec3(.5f, 1.f, .5f),
		glm::vec3(x, 0.f, z),
		mazeTextures[1],
		Type::Bush
	);
}

void MazeGenerator::PlaceFloor(const float& x, const float& z) {
	cubeCreator->AddCube(
		glm::vec3(x, 0.1f, z),
		glm::vec3(0.f, -0.5f, 0.f),
		mazeTextures[0],
		Type::Floor
	);
}