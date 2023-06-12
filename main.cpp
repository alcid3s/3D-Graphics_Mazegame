#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "MazeGenerator.h"
#include "ObjModel.h"
#include "GameObject.h"
#include "PlaneComponent.h"
#include "CubeComponent.h"
#include "PlayerComponent.h"
#include "CameraComponent.h"
#include "AudioComponent.h"
#include "FlashlightComponent.h"
#include "BoundingBoxComponent.h"
#include "EnemyComponent.h"
#include "AltarComponent.h"
#include "HUDComponent.h"
#include "ParticleComponent.h"
#include "GuiManager.h"
#include "Texture.h"
#include "FileEditor.h"
#include <memory>
#include <thread>
#include <atomic>

#include <iostream>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

// used to communicate between threads.
std::atomic<bool> bMazeGeneratedFinished(false);

// check if maze generation is started
bool bMazeGenerationStarted = false;

// check if all objects are initialised with components.
bool bObjectInitialised = false;

// window
GLFWwindow* window;

// GameObjects
std::shared_ptr<GameObject> player = nullptr;
std::shared_ptr<GameObject> endPoint = nullptr;
std::shared_ptr<GameObject> enemy = nullptr;
std::list<std::shared_ptr<GameObject>> objects;

// 3d model of enemy
ObjModel* enemyModel = nullptr;

// Textures
std::vector<Texture*> mazeTextures;

// Manager for maze generation
MazeGenerator* mazeGen;

// manager for ImGui
GuiManager* guiManager;

// used for deltatime
double lastFrameTime = 0;

// bool for debug purposes. Start with or without GUI.
constexpr bool activateGui = true;

// screen size
const int screenX = 1400, screenY = 800;

// width and length of maze
int mazeWidth = 10, mazeLength = 10;

// used to indicate when fileIO should be used.
bool bFinishedLevel = false;

// used to save maze. Will be cleared after maze is added to objects list.
std::vector<std::vector<std::shared_ptr<Cell>>> maze;

// File manager that handles fileIO
FileEditor* fileIO;

void init();
void update();
void draw();

void initObjects();
void generateMaze();
void enableFog(bool flag);
void updatePlayer(float deltaTime);
void updateGuiManager();

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(screenX, screenY, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	if (activateGui) {
		guiManager = new GuiManager(window, screenX, screenY);
		guiManager->init();
	}
	else {
		initObjects();
	}

	// prints stats of previous game to the console if a previous game exists.
	if (fileIO)
		fileIO->getStats();

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

	if (!activateGui)
		generateMaze();

	fileIO = new FileEditor("stats.txt");
}

void update()
{
	if (activateGui) {
		updateGuiManager();
		if (guiManager->menuType != MenuType::Playing) {
			return;
		}
		else if(fileIO)
			fileIO->begin();
	}

	if (bFinishedLevel && fileIO) {
		bFinishedLevel = false;
		fileIO->writeStats();
	}

	// Getting deltatime
	double currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	// Updating gameobjects
	for (auto& o : objects)
		o->update(deltaTime);

	// updating enemy
	enemy->update(deltaTime);

	// Updating player
	updatePlayer(deltaTime);

	// check if player reached endpoint and wants to go to next level. If so maze will be regenerated.
	if (endPoint && endPoint->getComponent<AltarComponent>()) {
		if (endPoint->getComponent<AltarComponent>()->endReached()) {
			mazeWidth++;
			mazeLength++;
			bMazeGenerationStarted = false;
			bFinishedLevel = true;
			guiManager->menuType = MenuType::Loading;
		}
	}
	else
		throw "endpoint not initialised";
}

void draw()
{
	if (activateGui) {
		guiManager->draw();
		if (guiManager->menuType != MenuType::Playing)
			return;
	}

	// Draw dark background
	glClearColor(.05f, .05f, .05f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make sure not all sides of the vertices are visible to the player
	glEnable(GL_DEPTH_TEST);

	// Set projection matrix
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glm::mat4 projection = glm::perspective(glm::radians(player->getComponent<CameraComponent>()->fov), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	auto cameraComponent = player->getComponent<CameraComponent>();

	// Setting matrices
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(cameraComponent->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	enableFog(true);

	// Drawing all gameobjects
	for (auto& o : objects)
		o->draw();

	// drawing enemy
	enemy->draw();

	// Drawing the flashlight, HUD and particleSystem of the player
	player->getComponent<HUDComponent>()->draw();
	player->getComponent<FlashlightComponent>()->draw();
	player->getComponent<ParticleComponent>()->draw();
}

/*
	Functions under here contain logic. But aren't convenient to put in a class.
*/

// Gives maze textures and shapes. Also instantiates player, altar and ambience gameObjects.
void initObjects() {

	// Added pointer of Texture to the vector.
	if (mazeTextures.empty()) {
		mazeTextures.push_back(new Texture("resource/textures/Floor4.png"));
		mazeTextures.push_back(new Texture("resource/textures/Bush_Texture4.png"));
		mazeTextures.push_back(new Texture("resource/textures/dirt.png"));
	}

	// Adding all gameobjects the generate function of maze created to the gameobjects list
	if (!maze.empty()) {
		for (auto row : maze) {
			for (auto obj : row) {
				if (obj->gameObject.type == Type::Wall || obj->gameObject.type == Type::Edge) {
					glm::vec3 minimal = glm::vec3(-.5f, 0, -.5f);
					glm::vec3 maximal = glm::vec3(.5f, 0, .5f);
					obj->gameObject.addComponent(std::make_shared<CubeComponent>(glm::vec3(1, 1, 1), mazeTextures[1]));
					obj->gameObject.addComponent(std::make_shared<BoundingBoxComponent>(minimal, maximal));
				}
				else if (obj->gameObject.type == Type::Floor || obj->gameObject.type == Type::Endpoint || obj->gameObject.type == Type::Spawnpoint) {
					obj->gameObject.addComponent(std::make_shared<PlaneComponent>(glm::vec3(1, 0, 1), mazeTextures[0]));
				}
				objects.push_back(std::make_shared<GameObject>(obj->gameObject));
			}
		}
	}
	else
		throw "Maze not generated";

	// clear maze because it's not needed anymore
	maze.clear();

	// Create player and sets it's position to the spawnpoint
	player = std::make_shared<GameObject>();
	player->type = Type::Player;

	// Adding components to the player
	player->addComponent(std::make_shared<PlayerComponent>(window));
	player->addComponent(std::make_shared<CameraComponent>(window));
	player->addComponent(std::make_shared<AudioComponent>(AudioType::AudioPlayer));
	player->addComponent(std::make_shared<FlashlightComponent>());
	player->addComponent(std::make_shared<HUDComponent>());
	player->addComponent(std::make_shared<ParticleComponent>(mazeTextures[2]));

	// give HUD access to the fov parameter
	player->getComponent<HUDComponent>()->setFov(&player->getComponent<CameraComponent>()->fov);

	// give HUD access to the running boolean and if this condition is met it'll do particle things.
	player->getComponent<ParticleComponent>()->setCondition(&player->getComponent<PlayerComponent>()->bIsRunning);

	glm::vec3 min = glm::vec3(-.1f, 0, -.1f);
	glm::vec3 max = glm::vec3(.1f, 0, .1f);
	player->addComponent(std::make_shared<BoundingBoxComponent>(min, max));

	player->position = mazeGen->spawnPoint;

	// loading object model in memory if it isn't loaded already
	if (!enemyModel)
		enemyModel = new ObjModel("resource/models/enemy/enemy.obj");

	// Creating an enemy and adding it to the objects list
	enemy = std::make_shared<GameObject>();
	enemy->addComponent(std::make_shared<EnemyComponent>(objects, enemyModel));
	enemy->position = mazeGen->enemySpawnTile->position;

	// adding ambient sound to the objects list
	auto ambience = std::make_shared<GameObject>();
	ambience->addComponent(std::make_shared<AudioComponent>(AudioType::Ambience));
	objects.push_back(ambience);

	// adding the endpoint object to the game. Y position -.499 so it stands directly on the floor. Needs player to check if player is near
	endPoint = std::make_shared<GameObject>();
	endPoint->position = mazeGen->endPoint;
	endPoint->position.y = -.499f;
	endPoint->addComponent(std::make_shared<AltarComponent>(window, player));
	objects.push_back(endPoint);

	bObjectInitialised = true;
}

// function to generate maze. Called from thread if GUI active
void generateMaze()
{
	// Initialise and draw a maze
	mazeGen = new MazeGenerator();
	maze = mazeGen->Generate(mazeWidth, mazeLength);

	bMazeGeneratedFinished = true;
}

// Enables fog into the world
void enableFog(bool flag) {
	if (flag) {
		tigl::shader->enableFog(true);
		tigl::shader->setFogColor(glm::vec3(.05f, .05f, .05f));
		tigl::shader->setFogLinear(1, 4);
		tigl::shader->setFogExp(1.5f);
	}
	else {
		tigl::shader->enableFog(false);
	}
}

// Functions to update the player
void updatePlayer(float deltaTime) {
	// getting player component
	auto playerComponent = player->getComponent<PlayerComponent>();

	// updating player
	player->update(deltaTime);

	// checking collision of player. If no collision occurs move is validated.
	playerComponent->checkCollision(objects);

	// Getting the rest of the components
	auto cameraComponent = player->getComponent<CameraComponent>();
	auto audioComponent = player->getComponent<AudioComponent>();
	auto flashlightComponent = player->getComponent<FlashlightComponent>();
	auto hudComponent = player->getComponent<HUDComponent>();

	// Letting the audio component know if there've been updates concerning the movement of the player
	audioComponent->bIsRunning = playerComponent->bIsRunning;
	audioComponent->bIsMoving = playerComponent->bMoving;
	audioComponent->bPlayOutOfBreathSound = playerComponent->bPlayOutOfBreathSound;

	// let flashLight know if player is running
	flashlightComponent->bIsRunning = playerComponent->bIsRunning;

	// let hudComponent know if player is moving or running
	hudComponent->setIsMoving(playerComponent->bMoving);
	hudComponent->setIsRunning(playerComponent->bIsRunning);

	// Change FOV according to the movement of the player
	cameraComponent->changeFOV(deltaTime, playerComponent->bIsRunning);
}

// updates all global variables in main according to the states.
void updateGuiManager()
{
	if (guiManager->menuType == MenuType::MainMenu) {
		guiManager->update();
		return;
	}
	else if (guiManager->menuType == MenuType::Loading && !bMazeGenerationStarted) {
		// setting to false if it was true, means regeneration started
		bMazeGeneratedFinished = false;
		bObjectInitialised = false;

		if (!objects.empty()) {
			objects.clear();
		}

		// maze generation ahs started so it won't start another time
		bMazeGenerationStarted = true;

		// creating thread initialising all variables
		std::thread thread(generateMaze);

		// detach so mainthread can run normally.
		thread.detach();
	}
	if (!bMazeGeneratedFinished) {
		return;
	}
	else if (bMazeGeneratedFinished && !bObjectInitialised) {
		initObjects();
	}
	else if (bMazeGeneratedFinished && bObjectInitialised && guiManager->menuType != MenuType::Playing) {
		guiManager->menuType = MenuType::Playing;
	}
}