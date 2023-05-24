#pragma once
#include "gameobjects/Gameobject.h"
#include <glm/gtc/matrix_transform.hpp>

enum Type;
class Texture;

class Plane : public GameObject {
public:
	Plane(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle = 0);
	~Plane();

	void drawPlanePT();
	void drawPlanePC();

	void update() override;
	void draw() override;
private:

};
