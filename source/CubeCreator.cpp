#include "CubeCreator.h"
#include "tigl.h"
using tigl::Vertex;

std::vector<Cube> cubes;

CubeCreator::CubeCreator() {
    if (!tigl::shader)
        tigl::init();
}

void CubeCreator::AddCube(const glm::vec3& transform, const glm::vec3& translate) {
    Cube cube;
    cube.transform = transform;
    cube.translate = translate;

    cubes.push_back(cube);
    tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), cube.translate));
    DrawCube(cube.transform);
}

void CubeCreator::DrawCube(const glm::vec3& size) {
    tigl::begin(GL_QUADS);
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));

    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));

    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));

    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));

    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));

    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));

    tigl::end();
}

std::vector<Cube> CubeCreator::getCubes()
{
    return cubes;
}