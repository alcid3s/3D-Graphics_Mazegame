#pragma once
#include <glm/gtc/matrix_transform.hpp>

class Texture;
enum Type;

class Gameobject {
public:
	Gameobject(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle);
	~Gameobject();

	// abstract functions
	virtual void update() = 0;
	virtual void draw() = 0;

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