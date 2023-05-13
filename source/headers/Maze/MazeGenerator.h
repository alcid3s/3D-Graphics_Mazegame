#pragma once
#include <vector>
#include "enumType.h"

class MazeGenerator {
public:
	MazeGenerator();
	~MazeGenerator();

	void Generate(const int& x = 10, const int& y = 10);
	void DrawMaze();
private:
	int x,y;
	std::vector<std::vector<Type>> maze;

	void PlaceBush(const float& x, const float& z);
	void PlaceFloor(const float& x, const float& z);
};