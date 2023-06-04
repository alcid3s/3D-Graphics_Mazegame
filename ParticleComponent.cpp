#include "ParticleComponent.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ParticleComponent::ParticleComponent(const int& numParticles) : numParticles(numParticles)
{

}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::init() {
	if (particles.empty()) {
		for (int i = 0; i < numParticles; ++i)
		{
			glm::vec3 position = -gameObject->position;

			position.z += 1.f;
			position.y = 1.f;

			if (position.z == 10.f) {
				position.z = 8;
			}

			Particle particle;
			particle.velocity = glm::vec3(0.f, 0.f, 0.f);
			particle.vert = Vertex::PT(position, generateColor());

			std::cout << "particle " << i << " (" << position.x << "," << position.y << "," << position.z << "\n";

			particles.push_back(particle);
		}
	}
}

glm::vec4 ParticleComponent::generateColor() {
	return glm::vec4(
		static_cast<float>(1.f),
		static_cast<float>(1.f),
		static_cast<float>(1.f), 1.f
	);
}

void ParticleComponent::update(float deltaTime)
{
	init();
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].vert.position += particles[i].velocity;
	}
}

void ParticleComponent::draw()
{
	tigl::begin(GL_POINT);

	for (const auto& p : particles) {
		tigl::shader->enableColor(true);
		glPointSize(10.f);
		tigl::addVertex(p.vert);
	}
	tigl::end();
	tigl::shader->enableColor(false);
}