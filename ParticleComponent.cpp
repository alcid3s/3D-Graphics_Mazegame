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

			particle.x = 1.f;

			glm::vec3 velocity = glm::vec3(0.f);

			//int randomVelocity = 0;// (((rand() % 4) + 1) / 10) + 1;
			//int direction = rand();

			//float parabolaY = a * (particle.x * particle.x) + b * particle.x + c;

			//if (direction % 5 == 0) {
			//	velocity = glm::vec3(-randomVelocity, parabolaY, -randomVelocity);
			//}
			//else if (direction % 4 == 0) {
			//	velocity = glm::vec3(randomVelocity, parabolaY, -randomVelocity);
			//}
			//else if (direction % 3 == 0) {
			//	velocity = glm::vec3(randomVelocity, parabolaY, randomVelocity);
			//}
			//else {
			//	velocity = glm::vec3(-randomVelocity, parabolaY, randomVelocity);
			//}

			particle.velocity = velocity;
			particle.originalVelocity = velocity;

			std::cout << "velocity for particle: " << i << " is (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";

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

void ParticleComponent::update(float deltaTime)
{
	init();
	//for (int i = 0; i < particles.size(); i++)
	//{
	//	for (int j = 0; j < 4; j++) {
	//			particles[i].vert[j].position.x = particles[i].velocity.x;
	//			particles[i].vert[j].position.y = particles[i].velocity.y;
	//			particles[i].vert[j].position.z = particles[i].velocity.z;

	//			// updates velocity
	//			particles[i].x -= 0.001f;
	//			float parabolaY = a * (particles[i].x * particles[i].x) + b * particles[i].x + c;
	//			particles[i].velocity.y = parabolaY;

	//			// reset particle position
	//			if (particles[i].vert[j].position.y < -5.f) {
	//				std::cout << "reset\n";
	//				particles[i].vert[0].position = particles[i].originalVert[0].position;
	//				particles[i].vert[1].position = particles[i].originalVert[1].position;
	//				particles[i].vert[2].position = particles[i].originalVert[2].position;
	//				particles[i].vert[3].position = particles[i].originalVert[3].position;

	//				particles[i].velocity = particles[i].originalVelocity;

	//				particles[i].x = 1.f;
	//				break;
	//			}
	//	}
	//	if (i == 0) {
	//		std::cout << "particle 0: (" << particles[i].vert[0].position.x << "," << particles[i].vert[0].position.y << "," << particles[i].vert[0].position.z << "), x: " << particles[i].x << "\n";
	//	}
	// }
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