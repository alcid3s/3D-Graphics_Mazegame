#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FpsCam.h"
#include "CubeCreator.h"
#include <vector>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* cam;
CubeCreator* cubeCreator;

std::vector<Texture> textures;

int width = 1400, height = 800;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

    // creating a floor
    cubeCreator->AddCube(
        glm::vec3(100.f, 0.1f, 100.f), 
        glm::vec3(0.f, -2.f, 0.f), 
        textures[0], 
        Type::Floor
    );

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

    return 0;
}

void init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            ::width = width;
            ::height = height;
        });

    cam = new FpsCam(window);
    cubeCreator = new CubeCreator();
    textures.push_back(Texture("resource/textures/Floor4.png"));
    // textures.push_back(Texture("resource/textures/Bush_Texture.png"));
}

void update() {
    cam->update(window);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(80.0f), (float)width / height, 0.1f, 100.0f));
    tigl::shader->setViewMatrix(cam->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);
    tigl::shader->enableLighting(false);
    tigl::shader->enableFog(false);

    glDisable(GL_DEPTH_TEST);
    tigl::shader->enableTexture(true);

    // draw all cubes
    cubeCreator->DrawCubes();
}