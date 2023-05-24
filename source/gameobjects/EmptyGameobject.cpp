#include "gameobject/EmptyGameobject.h"

EmptyGameobject::EmptyGameobject(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle)
	: GameObject(transf, rot, transl, tex, textMulti, angle)
{
}

void EmptyGameobject::update()
{
}

void EmptyGameobject::draw()
{
}
