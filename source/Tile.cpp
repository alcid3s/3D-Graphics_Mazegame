#include "Tile.h"
#include "modelLoader/ObjModel.h"
#include "headers/components/CubeComponent.h"
//#include "headers/components/PlaneComponent.h"
#include "headers/components/Component.h"
#include "enumType.h"

Tile::Tile(DrawComponent *drawObject, Type type, glm::vec3 position, bool visited) : 
	drawObject(drawObject), type(type), position(position), visited(visited)
{

}

Tile::~Tile() {

}

void Tile::setDrawComponent(DrawComponent* drawObject) {
	this->drawObject = drawObject;
}

void Tile::setModel(ObjModel* model) {
	this->model = model;
}

void Tile::draw() {
	drawObject->draw();

	if (model) {
		model->draw();
	}
}