#pragma once
#include "gameobjects/Gameobject.h"
class EmptyGameobject : public GameObject {
public:
	EmptyGameobject(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle = 0);
	~EmptyGameobject();

	void update() override;
	void draw() override;
};