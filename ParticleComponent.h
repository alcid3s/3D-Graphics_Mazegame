#pragma once
#include "DrawComponent.h"
#include "glm/glm.hpp"
#include <vector>
#include "tigl.h"
using tigl::Vertex;

struct Particle {
	Vertex vert;
	glm::vec3 velocity;
};

class ParticleComponent : public DrawComponent {
public:
	ParticleComponent(const int& numParticles = 5);
	~ParticleComponent();

	void update(float deltaTime) override;
	void draw() override;
protected:
	std::vector<Particle> particles;

	void init();
private:
	int numParticles;
private:
	glm::vec4 generateColor();
};