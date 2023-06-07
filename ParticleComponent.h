#pragma once
#include "DrawComponent.h"
#include "glm/glm.hpp"
#include <vector>
#include <array>
#include "tigl.h"
using tigl::Vertex;

struct Particle {
	std::array<Vertex, 4> vert;
	std::array<Vertex, 4> originalVert;

	glm::vec3 velocity;
	glm::vec3 originalVelocity;
	float x;
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

	// variables used to calculate the parabola of the particle
	const float a = -5.f, b = 0.f, c = 5.f;
private:
	glm::vec4 generateColor();
	glm::mat4 changeModelMatrix(bool flag);
};