#include "HUDComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/compatibility.hpp"
#include "Texture.h"
#include "GameObject.h"

HUDComponent::HUDComponent(std::string path) : texture(new Texture(path))
{
	// size for hud
	const float width = 0.295f;
	const float height = 0.168f;

	// vertices for quad of hud
	verts.push_back(tigl::Vertex::PT(glm::vec3(-width, -height, 0.0f), glm::vec2(0, 0)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(width, -height, 0.0f), glm::vec2(1, 0)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(width, height, 0.0f), glm::vec2(1, 1)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(-width, height, 0.0f), glm::vec2(0, 1)));
}

HUDComponent::~HUDComponent()
{

}

#include <iostream>
void HUDComponent::update(float deltaTime)
{
	if (texture) {
		glm::mat4 ret(1.0f);
		glm::vec3 position = -gameObject->position;

		// position of player
		ret = glm::translate(ret, position);

		// rotate hud
		ret = glm::rotate(ret, -gameObject->rotation.y, glm::vec3(0, 1, 0));

		// rotate hud around x-axis
		ret = glm::rotate(ret, -gameObject->rotation.x, glm::vec3(1, 0, 0));

		float zDistance = -.2f;

		if (bIsRunning) {
			// scale the HUD must change towards.
			float targetScale = glm::tan(glm::radians(*this->fov * 0.5f)) * 2.0f;

			// calculates current scale
			float currentScale = glm::length(glm::vec3(ret[0][0], ret[0][1], ret[0][2]));

			// linear interpolation (smooth transition) between current and target scale
			float scaleFactor = glm::lerp(currentScale, targetScale, deltaTime * 10.0f);

			// matrix is scaled accordingly
			ret = glm::scale(ret, glm::vec3(scaleFactor, scaleFactor, 1.0f));
		}
		else if (bIsMoving) {
			zDistance -= .01f;
			std::cout << "zDistance when moving: " << zDistance << " \n";
		}

		// move HUD slightly in front of player
		position = glm::vec3(0.0f, 0.0f, zDistance);

		// place HUD
		ret = glm::translate(ret, position);

		this->mat = ret;
	}
}

void HUDComponent::draw()
{
	if (texture) {
		tigl::shader->setModelMatrix(this->mat);
		bindHUD();

		// making texture transparent
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		texture->bind();
		tigl::shader->enableTexture(true);
		tigl::drawVertices(GL_QUADS, verts);
		tigl::shader->enableTexture(false);
		texture->unbind();
		unbindHUD();
	}
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
