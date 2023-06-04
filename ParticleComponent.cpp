#include "ParticleComponent.h"
#include "GameObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


ParticleComponent::ParticleComponent(const int& numParticles) : numParticles(numParticles)
{
	particles.resize(numParticles);
}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::update(float deltaTime)
{
	static bool initialised = false;
	if (!initialised) {
		initialised = true;
		glm::vec3 pos = -gameObject->position;
		pos.z -= 1.f;
		for (int i = 0; i < numParticles; ++i)
		{
			glm::vec4 color = glm::vec4(
				static_cast<float>(rand()) / 255,
				static_cast<float>(rand()) / 255,
				static_cast<float>(rand()) / 255, 1.f
			);
			auto tuple = std::make_tuple(glm::vec3(1.f, 1.f, 1.f), tigl::Vertex::PT(pos, color));
			particles.push_back(tuple);
		}

	}
	for (int i = 0; i < particles.size(); i++)
	{
		// get current position of particle
		glm::vec3& particlePosition = std::get<1>(particles[i]).position;

		// update particle position with velocity * deltaTime
		particlePosition += std::get<0>(particles[i]) * deltaTime;
	}
}

void ParticleComponent::draw()
{
	std::vector<tigl::Vertex> verts;
	for (const auto& p : particles) {
		auto& vert = std::get<1>(p);
		verts.push_back(vert);
	}

	glPointSize(1.f);
	tigl::shader->enableColor(true);

	tigl::drawVertices(GL_POINT, verts);

	tigl::shader->enableColor(false);
}
