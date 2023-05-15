#include "gameobjects/Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include "enumType.h"
#include "Texture.h"
#include "tigl.h"
using tigl::Vertex;

Cube::Cube(glm::vec3 transf, glm::vec3 rot, glm::vec3 transl, Texture* tex, int textMulti) 
    : Gameobject(), transform(transf), rotate(rot), translate(transl), texture(tex), textureMulti(textMulti)
{

}

Cube::~Cube()
{
}

void Cube::update() {

}

void Cube::draw() {
    if (texture) {
        texture->Bind();
        DrawCubePT();
        texture->Unbind();
    }
    else
        DrawCubePC();
}

void Cube::DrawCubePT() {
    tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), translate));
    tigl::shader->enableTexture(true);
    tigl::begin(GL_QUADS);

    // achterkant z
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    // voorkant z
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    // onderkant y
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    // bovenkant y
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    // linkerkant x
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    // rechterkant x
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec2(0, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec2(1 * textureMulti, 0)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec2(0, 1 * textureMulti)));
    tigl::addVertex(Vertex::PT(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec2(1 * textureMulti, 1 * textureMulti)));

    tigl::end();
    tigl::shader->enableTexture(false);
}

void Cube::DrawCubePC() {
    tigl::shader->enableColor(true);
    tigl::begin(GL_QUADS);

    // achterkant z
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));

    // voorkant z
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));

    // onderkant y
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));

    // bovenkant y
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));

    // linkerkant x
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));

    // rechterkant x
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, -transform.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, -transform.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, transform.y / 2, transform.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(transform.x / 2, -transform.y / 2, transform.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));

    tigl::end();
    tigl::shader->enableColor(false);
}