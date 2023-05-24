#include "headers/components/PlaneComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"

PlaneComponent::PlaneComponent(glm::vec3 transform, glm::vec3 translation, Texture* texture, int textureMultiplier)
	: textureMultiplier(textureMultiplier), translate(translate), texture(texture){
	if (texture) {
		verts.push_back(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
		verts.push_back(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMultiplier, 0)));
		verts.push_back(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMultiplier)));
		verts.push_back(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(1 * textureMultiplier, 1 * textureMultiplier)));
	}
	else {
		verts.push_back(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		verts.push_back(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		verts.push_back(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		verts.push_back(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}
}

PlaneComponent::~PlaneComponent() {

}

void PlaneComponent::draw() {
	tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), translate));
	tigl::begin(GL_QUADS);
	if (texture) {
		tigl::shader->enableTexture(true);
		texture->bind();
		tigl::drawVertices(GL_QUADS, verts);
		texture->unbind();
		tigl::shader->enableTexture(false);
	}
	else
		tigl::drawVertices(GL_QUADS, verts);
	tigl::end();
}