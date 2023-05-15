#include "Tile.h"
#include "gameobjects/Gameobject.h"
#include "gameobjects/Cube.h"
#include "gameobjects/Plane.h"
#include "enumType.h"

Tile::Tile(Gameobject *gameobject, Type type) : gameobject(gameobject), type(type)
{

}

Tile::~Tile() {

}

void Tile::draw() {
	gameobject->draw();
}