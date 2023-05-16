#include "gameobjects/Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "enumType.h"
#include "Texture.h"
#include "tigl.h"
using tigl::Vertex;

Plane::Plane(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti, float angle)
	: Gameobject(transf, rot, transl, tex, textMulti, angle)
{

}

Plane::~Plane()
{

}

void Plane::drawPlanePT()
{
	tigl::shader->enableTexture(true);
	tigl::begin(GL_QUADS);
	
	// drawing plane
	tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
	tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
	tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
	tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

	tigl::end();
	tigl::shader->enableTexture(false);
}

void Plane::drawPlanePC()
{
	tigl::shader->enableColor(true);
	tigl::begin(GL_QUADS);

	// drawing plane
	tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
	tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
	tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
	tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));

	tigl::end();
}

void Plane::update()
{

}

void Plane::draw()
{
	tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), translate));
	if(angle != 0)
		tigl::shader->setModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotate));

	if (texture) {
		texture->Bind();
		drawPlanePT();
		texture->Unbind();
	}
	else
		drawPlanePC();
}