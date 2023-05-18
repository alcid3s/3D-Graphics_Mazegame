#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FpsCam.h"
#include "Maze/MazeGenerator.h"
#include "LoadingScreen.h"
#include <thread>
#include <atomic>
#include <vector>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* cam;
MazeGenerator* mazeGen;
LoadingScreen* loadingScreen;

int width = 1400, height = 800;
double lastFrameTime = 0;
bool creatingMaze = false;

// used to communicate between threads.
std::atomic<bool> mazeGenerated(false);

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

    loadingScreen = new LoadingScreen();
    mazeGen = new MazeGenerator();
    cam = new FpsCam(window);
}

void generateMaze(int width, int height) {

    // generate a maze
    mazeGen->Generate(width, height);

    // set camera on spawnpoint
    cam->position = &mazeGen->spawnPoint;

    // atomic boolean set to true
    mazeGenerated = true;
}

void update() {
    if (!creatingMaze) {
        creatingMaze = true;

        // create thread to create maze. Because this can take a while depending on the size.
        std::thread mazeThread(generateMaze, 10, 10);

        // detach so mainthread can run normally.
        mazeThread.detach();
    }

    if (!mazeGenerated) {
        loadingScreen->update();
    }
    else {
        double frameTime = glfwGetTime();
        float deltaTime = lastFrameTime - frameTime;
        lastFrameTime = frameTime;

        cam->update(window, deltaTime);
    }
}

glm::vec3 color = glm::vec3(0.05f, 0.05f, 0.05f);
void draw() {

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // setting model matrix.
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    // to don't get a weird effect and all faces drawn to the screen.
    glEnable(GL_DEPTH_TEST);

    // while the maze generation thread is busy
    if (!mazeGenerated) {

        // set projection matrix. Influences Fov.
        tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(80.0f), (float)width / height, 0.1f, 100.0f));

        color = glm::vec3(0.f, 0.f, 0.f);
        tigl::shader->setViewMatrix(loadingScreen->GetMatrix());
        loadingScreen->draw();
    }

    // maze is generated.
    else {
        color = glm::vec3(0.05f, 0.05f, 0.05f);
        tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(cam->GetFov()), (float)width / height, 0.1f, 100.0f));
        tigl::shader->setViewMatrix(cam->getMatrix());
        mazeGen->DrawMaze();
    }
}