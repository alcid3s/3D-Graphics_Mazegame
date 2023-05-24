#pragma once
#include "headers/components/DrawComponent.h"
#include "tigl.h"
#include <vector>
using tigl::Vertex;

class Texture;

class PlaneComponent : public DrawComponent
{
	std::vector<Vertex> verts;
public:
	PlaneComponent(glm::vec3 transform, glm::vec3 translation, Texture* texture, int textureMultiplier = 1);
	~PlaneComponent();

	virtual void draw() override;

	inline glm::vec3 getTranslate() const { return translate; }
private:
	glm::vec3 translate;
	Texture* texture;
	int textureMultiplier;
};