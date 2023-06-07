#include "ParticleComponent.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "CameraComponent.h"

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
			glm::vec3 position = glm::vec3(0.f);

			Particle particle;

			// sets size and origin of particle
			particle.vert[0] = Vertex::PC(position + glm::vec3(-.05, 0, -.05), generateColor());
			particle.vert[1] = Vertex::PC(position + glm::vec3(.05, 0, -.05), generateColor());
			particle.vert[2] = Vertex::PC(position + glm::vec3(.05, 0, .05), generateColor());
			particle.vert[3] = Vertex::PC(position + glm::vec3(-.05, 0, .05), generateColor());

			particle.originalVert[0] = particle.vert[0];
			particle.originalVert[1] = particle.vert[1];
			particle.originalVert[2] = particle.vert[2];
			particle.originalVert[3] = particle.vert[3];

			particle.x = .5f;

			glm::vec3 velocity = glm::vec3(0.f);

			int randomVelocity = 0; // (((rand() % 4) + 1) / 10) + 1;

			float parabolaY = calculateParabola(particle.x);

			//switch (rand() % 4) {
			//case 3:
			//	velocity = glm::vec3(-randomVelocity, parabolaY, -randomVelocity);
			//	break;
			//case 2:
			//	velocity = glm::vec3(randomVelocity, parabolaY, -randomVelocity);
			//	break;
			//case 1:
			//	velocity = glm::vec3(randomVelocity, parabolaY, randomVelocity);
			//	break;
			//case 0:
			//	velocity = glm::vec3(-randomVelocity, parabolaY, randomVelocity);
			//	break;
			//}

			particle.velocity = velocity;
			particle.originalVelocity = velocity;
			particle.previousVelocity = velocity;

			particles.push_back(particle);

			if (i == 0)
				std::cout << "start of particle 0: (" << particles[i].vert[0].position.x << "," << particles[i].vert[0].position.y << "," << particles[i].vert[0].position.z << ")\n";
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

float ParticleComponent::calculateParabola(const float& x) {
	return (a * (x * x)) + (b * x) + (c);
}

void ParticleComponent::resetParticle(Particle& particle) {
	std::cout << "resetting particle\n";
	particle.vert[0].position = particle.originalVert[0].position;
	particle.vert[1].position = particle.originalVert[1].position;
	particle.vert[2].position = particle.originalVert[2].position;
	particle.vert[3].position = particle.originalVert[3].position;

	particle.velocity = particle.originalVelocity;
	particle.previousVelocity = particle.originalVelocity;

	particle.x = .5f;
}

void ParticleComponent::update(float deltaTime)
{
	init();
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = 0; j < 4; j++) {

			// updates position with velocity
			particles[i].vert[j].position += (particles[i].velocity - particles[i].previousVelocity);

			// updates velocity
			particles[i].previousVelocity = particles[i].velocity;
			particles[i].x -= 0.001f;
			particles[i].velocity.y = calculateParabola(particles[i].x);

			// reset particle position
			if (particles[i].vert[j].position.y < -.5f) {
				resetParticle(particles[i]);
				break;
			}
		}
		if (i == 0) {
			std::cout << "particle 0: (" << particles[i].vert[0].position.x << "," << particles[i].vert[0].position.y << "," << particles[i].vert[0].position.z << "), x: " << particles[i].x << "\n";
		}
	}
}

glm::mat4 ParticleComponent::changeModelMatrix(bool flag) {
	auto camComp = gameObject->getComponent<CameraComponent>();

	if (flag && camComp) {
		glm::mat4 modelMatrix = glm::mat4(1.f);
		glm::vec3 pos = -gameObject->position;
		pos.y = -1.49;
		modelMatrix = glm::translate(modelMatrix, pos);
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.x, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.y, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.z, glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, gameObject->scale);

		//glm::mat4 viewMat = camComp->getMatrix();

		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < 3; j++) {
		//		viewMat[i][j] = i == j ? 1.f : 0.f;
		//	}
		//}

		//tigl::shader->setViewMatrix(viewMat);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				modelMatrix[i][j] = i == j ? 1.f : 0.f;
			}
		}

		// returning modelMatrix
		return modelMatrix;
	}
	else if (camComp) {

		// returning viewMatrix
		return camComp->getMatrix();
	}

	return glm::mat4(1.f);
}

void ParticleComponent::draw()
{
	glm::vec3 pos = -gameObject->position;
	pos.y += -.3f;

	glm::mat4 ret(1.f);
	ret = glm::translate(ret, pos);

	tigl::shader->setModelMatrix(ret);

	tigl::shader->enableColor(true);
	tigl::begin(GL_QUADS);
	for (const auto& p : particles) {
		for (int i = 0; i < 4; i++) {
			tigl::addVertex(p.vert[i]);
		}
	}
	tigl::end();
	tigl::shader->enableColor(false);
}