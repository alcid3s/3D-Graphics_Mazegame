#pragma once

enum Type;
class Gameobject;

class Tile {
public:
	Tile(Gameobject *, Type type);
	~Tile();

	void draw();
private:
	Tile *neighbours[4];
	Type type;
	Gameobject* gameobject;
};