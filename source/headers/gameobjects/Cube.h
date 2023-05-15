#pragma once
#include "gameobjects/Gameobject.h"
#include <glm/gtc/matrix_transform.hpp>

enum Type;
class Texture;

class Cube : public Gameobject {
public:
	Cube(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti);
	~Cube();

	void DrawCubePT();
	void DrawCubePC();

	void update() override;
	void draw() override;
private:
	glm::vec3 transform;
	glm::vec3 rotate;
	glm::vec3 translate;

	Texture* texture;
	Type type;

	int textureMulti;
};