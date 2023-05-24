#pragma once

#include "headers/components/DrawComponent.h"
#include "tigl.h"
#include <vector>
using tigl::Vertex;

class Texture;

class CubeComponent : public DrawComponent
{
	std::vector<Vertex> verts;
public:
	CubeComponent(glm::vec3 transform, glm::vec3 translation, Texture* texture, int textureMultiplier = 1);
	~CubeComponent();

	virtual void draw() override;

	glm::vec3 translate;
private:
	
	Texture* texture;
	int textureMultiplier;
};