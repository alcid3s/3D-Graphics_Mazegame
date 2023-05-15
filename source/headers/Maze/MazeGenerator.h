#pragma once
#include <vector>
#include "enumType.h"

class CubeCreator;
class Texture;

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const int& x = 10, const int& y = 10);
	void DrawMaze();
private:
	int x,y;
	std::vector<std::vector<Type>> maze;

	CubeCreator* cubeCreator;
	std::vector<Texture*> mazeTextures;

	void PlaceBush(const float& x, const float& z);
	void PlaceFloor(const float& x, const float& z);
};