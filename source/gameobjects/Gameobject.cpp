#include "gameobjects/Gameobject.h"
#include "Texture.h"
#include "enumType.h"

GameObject::GameObject(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle)
	: transform(transf), rotate(rot), translate(transl), texture(tex), textureMulti(textMulti), angle(angle)
{

}

GameObject::~GameObject()
{
}
