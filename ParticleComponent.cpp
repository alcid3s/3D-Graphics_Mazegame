#include "ParticleComponent.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "CameraComponent.h"

ParticleComponent::ParticleComponent(Texture* texture, const int& numParticles) : numParticles(numParticles), texture(texture), bCondition(nullptr)
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

			// hardcoding the size of a particle
			glm::vec3 size[4] = {
				glm::vec3(-.01, 0, -0.01),
				glm::vec3(.01, 0, -0.01),
				glm::vec3(.01, 0, .01),
				glm::vec3(-.01, 0, .01)
			};

			// add vertices to particle.
			particle.vert[0] = Vertex::PC(position + size[0], generateColor());
			particle.vert[1] = Vertex::PC(position + size[1], generateColor());
			particle.vert[2] = Vertex::PC(position + size[2], generateColor());
			particle.vert[3] = Vertex::PC(position + size[3], generateColor());

			// Original values, so particle can respawn on exact same position
			particle.originalVert[0] = particle.vert[0];
			particle.originalVert[1] = particle.vert[1];
			particle.originalVert[2] = particle.vert[2];
			particle.originalVert[3] = particle.vert[3];

			// x for parabola
			particle.x = .5f;

			// creating random velocity in a direction
			glm::vec3 velocity = glm::vec3(0.f);
			float randomVelocityX = static_cast<float>(rand() % 100 - 50) / spread;
			float randomVelocityY = static_cast<float>(rand() % 100 - 50) / spread;
			switch (rand() % 4) {
			case 3:
				velocity = glm::vec3(-randomVelocityX, 0, -randomVelocityY);
				break;
			case 2:
				velocity = glm::vec3(randomVelocityX, 0, -randomVelocityY);
				break;
			case 1:
				velocity = glm::vec3(randomVelocityX, 0, randomVelocityY);
				break;
			case 0:
				velocity = glm::vec3(-randomVelocityX, 0, randomVelocityY);
				break;
			default:
				throw "error creating random velocity in ParticleComponent";
			}

			particle.velocity = velocity;
			particle.originalVelocity = velocity;
			particle.previousVelocity = velocity;

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

float ParticleComponent::calculateParabola(const float& x) {
	return (a * (x * x)) + (b * x) + (c);
}

void ParticleComponent::resetParticle(Particle& particle) {
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
	if (*this->bCondition) {
		for (int i = 0; i < particles.size(); i++)
		{
			for (int j = 0; j < 4; j++) {

				// updates position of y with velocity
				particles[i].vert[j].position.y += (particles[i].velocity.y - particles[i].previousVelocity.y);

				particles[i].vert[j].position.x += particles[i].velocity.x;
				particles[i].vert[j].position.z += particles[i].velocity.z;

				// updates velocity
				particles[i].previousVelocity = particles[i].velocity;
				particles[i].x -= particleXSpeed;
				particles[i].velocity.y = calculateParabola(particles[i].x);

				// reset particle position
				if (particles[i].vert[j].position.y < -.5f) {
					resetParticle(particles[i]);
					break;
				}
			}
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
	if (*this->bCondition) {
		glm::vec3 pos = -gameObject->position;
		pos.y += -.3f;

		glm::mat4 ret(1.f);
		ret = glm::translate(ret, pos);

		tigl::shader->setModelMatrix(ret);
		tigl::shader->enableColor(true);
		tigl::shader->enableTexture(true);

		texture->bind();
		tigl::begin(GL_QUADS);
		for (const auto& p : particles) {
			for (int i = 0; i < 4; i++) {
				tigl::addVertex(p.vert[i]);
			}
		}
		tigl::end();
		texture->unbind();
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(false);
	}
}