#include "gameobjects/Gameobject.h"
#include "headers/components/Component.h"
#include "headers/components/DrawComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include <memory>

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (drawComponent == nullptr)
	{
		drawComponent = dynamic_pointer_cast<DrawComponent>(component);
	}
}

void GameObject::draw(const glm::mat4& parentMatrix)
{
	if (!drawComponent)
		return;

	glm::mat4 modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, translate);
	modelMatrix = glm::rotate(modelMatrix, rotate.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotate.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotate.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, transform);

	tigl::shader->setModelMatrix(modelMatrix);
	drawComponent->draw();
}

void GameObject::update(float elapsedTime)
{
	for (auto& c : components)
	{
		c->update(elapsedTime);
	}
}