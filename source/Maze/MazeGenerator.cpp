#include "Maze/MazeGenerator.h"
#include "CubeCreator.h"
#include "Texture.h"

#include <iostream>

MazeGenerator::MazeGenerator() {
	cubeCreator = new CubeCreator();

	mazeTextures.push_back(new Texture("resource/textures/Floor4.png"));
	mazeTextures.push_back(new Texture("resource/textures/Bush_Texture.png"));
}

MazeGenerator::~MazeGenerator() {

}

void MazeGenerator::Generate(const int& x, const int& z) {
	PlaceFloor((float)x, (float)z);

	PlaceBush((x / 2) - .25f, (z / 2) - .25f);
	PlaceBush(-1 * (x / 2) - .25f, (z / 2) - .25f);
	PlaceBush((x / 2) - .25f, -1 * (z / 2) - .25f);
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