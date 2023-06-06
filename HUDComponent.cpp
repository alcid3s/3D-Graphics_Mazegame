#include "HUDComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/glm.hpp"
#include <cmath>

#include "Texture.h"
#include "GameObject.h"

// size for hud
const float width = 0.295f;
const float height = 0.168f;

HUDComponent::HUDComponent(std::string path) : texture(new Texture(path))
{
	// vertices for quad of hud
	verts.push_back(tigl::Vertex::PT(glm::vec3(-width, -height, 0.0f), glm::vec2(0, 0)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(width, -height, 0.0f), glm::vec2(1, 0)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(width, height, 0.0f), glm::vec2(1, 1)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(-width, height, 0.0f), glm::vec2(0, 1)));
}

HUDComponent::~HUDComponent()
{

}

void HUDComponent::update(float deltaTime)
{
	
}

#include <iostream>
void HUDComponent::updateHUD() {
	glm::mat4 ret = glm::mat4(1.0f);

	float zDistance = -.2f;

	// set HUD on gameObject position
	ret = glm::translate(ret, -gameObject->position);

	// rotate HUD
	ret = glm::rotate(ret, -gameObject->rotation.y, glm::vec3(0, 1, 0));
	ret = glm::rotate(ret, -gameObject->rotation.x, glm::vec3(1, 0, 0));

	if (bIsRunning) {
		// Get bigger according to FOV
		float scalingFactor = (mapValue(*this->fov) / 100) * 1.3;
		//std::cout << "max " << scalingFactor << "\n";
		ret = glm::scale(ret, glm::vec3(scalingFactor, scalingFactor, 1.0f));
	}
	else if (*this->fov > 75.f) {
		// Get smaller according to FOV
		float scalingFactor = (mapValue(*this->fov) / 100) * 1.3;
		//std::cout << "min " << scalingFactor << "\n";
		ret = glm::scale(ret, glm::vec3(scalingFactor, scalingFactor, 1.0f));
	}
	else if (bIsMoving) {
		zDistance -= .01f;
	}

	// place a bit in front of camera
	ret = glm::translate(ret, glm::vec3(0.0f, 0.0f, -.2f));

	tigl::shader->setModelMatrix(ret);
}

float HUDComponent::mapValue(float value) {
	return ((value - 75.0f) * (25.0f/ 34.0f)) + 100.0f;
}

void HUDComponent::draw()
{
	if (texture) {

		// called here because it no workey in update.
		updateHUD();

		bindHUD();

		// making texture transparent
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		texture->bind();
		tigl::shader->enableTexture(true);
		tigl::drawVertices(GL_QUADS, verts);
		tigl::shader->enableTexture(false);
		texture->unbind();

		glEnable(GL_DEPTH_TEST);
		unbindHUD();
	}
	else
		throw "Could not load HUD texture.";
}

void HUDComponent::bindHUD()
{
	// Change to 2D projection
	glMatrixMode(GL_PROJECTION);

	// Save current projection
	glPushMatrix();

	// Reset matrix
	glLoadIdentity();

	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void HUDComponent::unbindHUD()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
