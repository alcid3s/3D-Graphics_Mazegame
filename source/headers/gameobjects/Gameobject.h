#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class Component;
class Texture;
enum Type;

class GameObject {
	std::vector<std::shared_ptr<Component>> components;

public:
	GameObject(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle);
	~GameObject();

	// abstract functions
	virtual void update() = 0;
	virtual void draw() = 0;

	void addComponent(std::shared_ptr<Component> component);
	std::vector<std::shared_ptr<Component>> getComponents();

	// Getter functions
	inline glm::vec3 GetTransform() const { return transform; }
	inline glm::vec3 GetRotation() const { return rotate; }
	inline glm::vec3 GetTranslate() const { return translate; }
protected:
	glm::vec3 transform;
	glm::vec3 rotate;
	glm::vec3 translate;
	float angle;

	Texture* texture;

	int textureMulti;
};