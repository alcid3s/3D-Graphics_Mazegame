#include "CubeCreator.h"
#include "tigl.h"
#include "Texture.h"
using tigl::Vertex;

std::vector<Cube> cubes;

CubeCreator::CubeCreator() {
    if (!tigl::shader)
        tigl::init();
}

void CubeCreator::DrawCubes() {
    for (auto cube : cubes) {
        
        // set the matrix to the position the cube should be drawn.
        tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), cube.translate));

        // if the cube has a texture it should draw the texture on the cube. Otherwise just draw a cube.
        if (cube.texture != nullptr) {
            DrawCubePT(cube.transform, *cube.texture);
        } else {
            DrawCubePC(cube.transform);
        }
    }
}

void CubeCreator::AddCube(const glm::vec3& transform, const glm::vec3& translate, Texture& texture) {
    Cube cube;
    cube.transform = transform;
    cube.translate = translate;
    cube.texture = &texture;

    cubes.push_back(cube);
}

void CubeCreator::AddCube(const glm::vec3& transform, const glm::vec3& translate) {
    Cube cube;
    cube.transform = transform;
    cube.translate = translate;
    cube.texture = nullptr;
    cubes.push_back(cube);
}

void CubeCreator::DrawCubePT(const glm::vec3& size, Texture& texture) {
    tigl::shader->enableTexture(true);
    tigl::begin(GL_QUADS);

    // test for textures
    tigl::addVertex(Vertex::PT(glm::vec3(0, 0, 0), glm::vec2(0,0)));
    tigl::addVertex(Vertex::PT(glm::vec3(0, 1, 0), glm::vec2(0,1)));
    tigl::addVertex(Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1,1)));
    tigl::addVertex(Vertex::PT(glm::vec3(1, 0, 0), glm::vec2(1,0)));

    //// achterkant z
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec2(1, 1)));

    //// voorkant z
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec2(1, 1)));

    //// onderkant y
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec2(1, 0)));

    //// bovenkant y
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec2(1, 1)));

    //// linkerkant x
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec2(1, 1)));

    //// rechterkant x
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec2(0, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec2(1, 0)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec2(0, 1)));
    //tigl::addVertex(Vertex::PT(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec2(1, 1)));

    tigl::end();
    tigl::shader->enableTexture(false);
}

void CubeCreator::DrawCubePC(const glm::vec3& size) {
    tigl::shader->enableColor(true);
    tigl::begin(GL_QUADS);

    // achterkant z
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 1.0f, 0.9f, 1.0f)));

    // voorkant z
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f)));

    // onderkant y
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(1.0f, 0.5f, 1.0f, 1.0f)));

    // bovenkant y
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(1.0f, 0.8f, 1.0f, 1.0f)));

    // linkerkant x
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, size.y / 2, size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f)));

    // rechterkant x
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, -size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, size.y / 2, size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));
    tigl::addVertex(Vertex::PC(glm::vec3(size.x / 2, -size.y / 2, size.z / 2), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f)));

    tigl::end();
    tigl::shader->enableColor(false);
}

std::vector<Cube> CubeCreator::getCubes()
{
    return cubes;
}