#include "Tile.h"
#include "gameobjects/Gameobject.h"
#include "gameobjects/Cube.h"
#include "gameobjects/Plane.h"
#include "enumType.h"

Tile::Tile(Gameobject *gameobject, Type type, glm::vec3 position, bool visited) : gameobject(gameobject), type(type), position(position), visited(visited)
{

}

Tile::~Tile() {

}

void Tile::setGameobject(Gameobject* gameobject) {
	this->gameobject = gameobject;
}

void Tile::draw() {
	gameobject->draw();
}