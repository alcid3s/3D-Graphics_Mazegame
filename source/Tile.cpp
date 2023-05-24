#include "Tile.h"
#include "modelLoader/ObjModel.h"
#include "headers/components/Component.h"
#include "headers/components/CubeComponent.h"
#include "enumType.h"

Tile::Tile(DrawComponent *drawComponent, Type type, glm::vec3 position, bool visited) : 
	drawComponent(drawComponent), type(type), position(position), visited(visited)
{

}

Tile::~Tile() {

}

void Tile::setGameobject(DrawComponent* drawComponent) {
	this->drawComponent = drawComponent;
}

void Tile::setModel(ObjModel* model) {
	this->model = model;
}

void Tile::draw() {
	this->drawComponent->draw();

	if (model) {
		model->draw();
	}
		
}