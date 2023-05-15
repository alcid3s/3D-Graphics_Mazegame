#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FpsCam.h"
#include "Maze/MazeGenerator.h"
#include <vector>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* cam;
MazeGenerator* mazeGen;

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

    // generate a maze
    mazeGen->Generate();

    cam->position = &mazeGen->spawnPoint;

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

void init() {
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

    mazeGen = new MazeGenerator();
    cam = new FpsCam(window);
}

void update() {
    cam->update(window);
}

void draw() {

    glViewport(0, 0, width, height);
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(80.0f), (float)width / height, 0.1f, 100.0f));
    tigl::shader->setViewMatrix(cam->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    // to don't get a weird effect and all faces drawn to the screen.
    glEnable(GL_DEPTH_TEST);

    mazeGen->DrawMaze();
}