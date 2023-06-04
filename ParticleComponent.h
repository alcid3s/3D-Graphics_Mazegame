#pragma once
#include "DrawComponent.h"
#include "glm/glm.hpp"
#include <tuple>
#include <vector>
#include "tigl.h"

class ParticleComponent : public DrawComponent {
public:
	ParticleComponent(const int& numParticles = 1);
	~ParticleComponent();

	void update(float deltaTime) override;
	void draw() override;
protected:
	std::vector<std::tuple<glm::vec3, tigl::Vertex>> particles;

private:
	int numParticles;
};