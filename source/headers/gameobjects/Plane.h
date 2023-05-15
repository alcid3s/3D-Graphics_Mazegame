#pragma once
#include "gameobjects/Gameobject.h"
#include <glm/gtc/matrix_transform.hpp>

enum Type;
class Texture;

class Plane : public Gameobject {
public:
	Plane(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti);
	~Plane();

	void drawPlanePT();
	void drawPlanePC();

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
