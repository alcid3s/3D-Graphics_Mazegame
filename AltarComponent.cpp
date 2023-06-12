#include "AltarComponent.h"
#include "ObjModel.h"
#include "GameObject.h"
#include <iostream>

AltarComponent::AltarComponent(GLFWwindow* window, std::shared_ptr<GameObject> obj, std::string path) : model(new ObjModel(path)), player(obj), window(window)
{

}

AltarComponent::~AltarComponent()
{

}

void AltarComponent::update(float deltaTime)
{
	if (gameObject->position.x - minimalPlayerDistance < -player->position.x && gameObject->position.x + minimalPlayerDistance > -player->position.x) {
		if (gameObject->position.z - minimalPlayerDistance < -player->position.z && gameObject->position.z + minimalPlayerDistance > -player->position.z) {
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				this->bReachedEnd = true;
		}
	}
}

void AltarComponent::draw()
{
	if (model)
		model->draw();
}
