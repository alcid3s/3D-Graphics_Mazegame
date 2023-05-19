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
#include <tuple>
#include <iostream>
#include "SFML/Audio.hpp"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* cam;
MazeGenerator* mazeGen;
LoadingScreen* loadingScreen;

// sound controls
sf::Music backgroundAmbience;

// randomsounds the player could make at any moment.
std::vector<std::tuple<sf::Sound*, sf::SoundBuffer*>> randomSounds;
const int randomness = 1000;

int width = 1400, height = 800;
double lastFrameTime = 0;

int mazeSizeX = 10, mazeSizeZ = 10;

bool creatingMaze = false;

// used to communicate between threads.
std::atomic<bool> mazeGenerated(false);

void init();
void update();
void draw();

void soundsSetup();
void playRandomSound();

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

    soundsSetup();

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

    backgroundAmbience.openFromFile("resource/sounds/ambience.wav");
    backgroundAmbience.setVolume(5.f);
    backgroundAmbience.setPitch(1.f);
}

void generateMaze(int width, int height) {

    // generate a maze
    mazeGen->Generate(width, height);

    // set camera on spawnpoint
    cam->position = &mazeGen->spawnPoint;

    // give the endPoint to the camera. When cam is at endPoint game is won.
    cam->setEndpoint(mazeGen->endPoint);

    // atomic boolean set to true
    mazeGenerated = true;
}

void soundsSetup() {
    for (int i = 1; i <= 2; i++) {
        sf::Sound* sound = new sf::Sound();;
        sf::SoundBuffer* buffer = new sf::SoundBuffer();

        std::string file = "resource/sounds/characterSounds/randomSound" + std::to_string(i) + ".wav";

        std::cout << "load randomSound successfull: " << (buffer->loadFromFile(file) ? "true" : "false") << "\n";
        sound->setPitch(1.f);
        sound->setVolume(100.f);
        sound->setBuffer(*buffer);
        sound->setMinDistance(5.f);
        sound->setAttenuation(0.5f);
        randomSounds.push_back(std::make_tuple(sound, buffer));
    }
}

void playRandomSound() {

}
void update() {
    if (!creatingMaze) {
        creatingMaze = true;

        // setting to false if it was true
        mazeGenerated = false;

        // create thread to create maze. Because this can take a while depending on the size.
        std::thread mazeThread(generateMaze, mazeSizeX, mazeSizeZ);

        // detach so mainthread can run normally.
        mazeThread.detach();

        // play music
        backgroundAmbience.play();
    }

    if (!mazeGenerated) {

    // setting model matrix.
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    // to don't get a weird effect and all faces drawn to the screen.
    glEnable(GL_DEPTH_TEST);

        loadingScreen->update();
    }

    // all things that need updating in the maze must be here
    else {
        double frameTime = glfwGetTime();
        float deltaTime = lastFrameTime - frameTime;
        lastFrameTime = frameTime;

        cam->update(window, deltaTime);

        if (rand() % randomness == 0) {
            int randomPos = rand() % randomSounds.size();
            sf::Sound* sound = std::get<sf::Sound*>(randomSounds[randomPos]);

            if (sound->getStatus() != sf::Sound::Playing) {
                sf::Listener::setDirection(cam->position->x, cam->position->y, cam->position->z);

                sound->setPosition(cam->position->x, cam->position->y, cam->position->z);
                sound->play();
            }
        }
    }

    if (cam->endPointReached) {
        //exit(0);
    }
}

glm::vec3 color = glm::vec3(0.05f, 0.05f, 0.05f);
void draw() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(color.r, color.g, color.b, 1.0f);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
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