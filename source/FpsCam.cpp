#include "FpsCam.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "Tile.h"
#include "enumType.h"
#include "Maze/MazeGenerator.h"
#include <ctime>

#define maxRunningTime 4
#define maxRecoverTime 4

#define maxFlashlightOffTime 4

#include <iostream>
FpsCam::FpsCam(GLFWwindow* window) : fov(80.f) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	for (int i = 1; i <= 7; i++) {
		sf::Sound* sound = new sf::Sound();
		sf::SoundBuffer* buffer = new sf::SoundBuffer();

		std::string file = "resource/sounds/footsteps/wav/footstep" + std::to_string(i) + ".wav";

		std::cout << "load file successfull: " << (buffer->loadFromFile(file) ? "true" : "false") << "\n";
		sound->setPitch(1.f);
		sound->setVolume(20.f);
		sound->setBuffer(*buffer);
		sound->setMinDistance(5.f);
		sound->setAttenuation(0.5f);
		footsteps.push_back(std::make_tuple(*sound, *buffer));
	}

	outOfBreath = new sf::Sound();
	sf::SoundBuffer* buffer = new sf::SoundBuffer();

	std::cout << "load outOfBreath successfull: " << (buffer->loadFromFile("resource/sounds/characterSounds/outOfBreath.wav") ? "true" : "false") << "\n";

	outOfBreath->setPitch(1.f);
	outOfBreath->setVolume(40.f);
	outOfBreath->setBuffer(*buffer);
	outOfBreath->setMinDistance(5.f);
	outOfBreath->setAttenuation(0.5f);

	flashlight = new ObjModel("resource/models/flashlight/flashlight.obj");
}

glm::mat4 FpsCam::getMatrix() {
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, *position);
	return ret;
}

/*This function contains a bug. When the player keeps walking on edges the function wont
keep track of the current position anymore and then the player can walk through walls.*/
void FpsCam::move(float angle, float fac, float deltaTime) {
	if (!tile)
		return;

	const float edge = 0.5f;
	const float tolerance = 0.1f;

	float x = tile->GetPosition().x;
	float z = tile->GetPosition().z;

	float movementX = position->x + ((float)cos(rotation.y + glm::radians(angle)) * fac) * deltaTime;
	float movementZ = position->z + ((float)sin(rotation.y + glm::radians(angle)) * fac) * deltaTime;

	float posX = -position->x;
	float posZ = -position->z;

	bool moveX = true, moveZ = true;

	// if close to edge
	if (!(x - edge + tolerance < -position->x && -position->x < x + edge - tolerance && z - edge + tolerance < -position->z && -position->z < z + edge - tolerance)) {
		if (!closeToEdge) {
			closeToEdge = true;
			//std::cout << "close to edge\n";
			neighbours = GetNeighbours(this->tile);
		}
	}

	// not close to edge anymore
	else if (closeToEdge) {
		closeToEdge = false;
		//std::cout << "back on tile\n";
		neighbours.clear();
	}

	// player is close to edge. Check if it crossed the tile.
	if (!neighbours.empty() && closeToEdge) {
		for (int i = 0; i < neighbours.size(); i++) {
			float nx = neighbours.at(i)->GetPosition().x;
			float nz = neighbours.at(i)->GetPosition().z;

			// if is inside neighbor's tile and away from the edge of the tile.
			if ((nx - edge + tolerance < posX && nx + edge - tolerance > posX && nz - edge + tolerance < posZ && nz + edge - tolerance > posZ) && neighbours.at(i)->type == Type::Floor) {
				this->tile = neighbours.at(i);
				neighbours.clear();
				closeToEdge = false;
				//std::cout << "new tile: (" << nx << "," << nz << ")\n";
			}
			else if(neighbours.at(i)->type != Type::Floor && neighbours.at(i)->type != Type::Endpoint) {
				const float removedFromEdge = 0.05;
				if (i == Bearing::South) {
					if (nz - edge - tolerance <= posZ) {
						movementZ = nz - edge - tolerance - removedFromEdge;
						movementZ *= -1;
						//std::cout << "touched south wall\n";
					}
				}
				else if (i == Bearing::North) {
					if (nz + edge + tolerance >= posZ) {
						movementZ = nz + edge + tolerance + removedFromEdge;
						movementZ *= -1;
						//std::cout << "touched north wall\n";
					}
				}
				else if (i == Bearing::West) {
					if (nx + edge + tolerance >= posX) {
						movementX = nx + edge + tolerance + removedFromEdge;
						movementX *= -1;
						//std::cout << "touched west wall\n";
					}
				}
				else if (i == Bearing::East) {
					if (nx - edge - tolerance <= posX) {
						movementX = nx - edge - tolerance - removedFromEdge;
						movementX *= -1;
						//std::cout << " touched east wall\n";
					}
				}
			}
		}
	 }
	position->x = movementX;
	position->z = movementZ;
	PlayFootstep();
}

void FpsCam::update(GLFWwindow* window, float deltaTime) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	static double lastX = x;
	static double lastY = y;

	float tempX = rotation.x - (float)(lastY - y) / 100.f;

	if (tempX < 0.9f && tempX > -0.7f)
		rotation.x = tempX;

	rotation.y -= (float)(lastX - x) / 100.f;

	lastX = x;
	lastY = y;

	moveCam(window, .75f, deltaTime);
	changeFov(deltaTime);

	if (isAtEndpoint(0.9f)) {
		endPointReached = true;
	}

	MaxRunTime();
}

std::tuple<glm::mat4, glm::vec3> FpsCam::PositionFlashlight() {
	glm::mat4 ret(1.0f);
	glm::vec3 flashPos = -*this->position;

	// set matrix on cams current position.
	ret = glm::translate(ret, flashPos);

	// rotate camera in right direction.
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, -rotation.y + 2.7f, glm::vec3(0, 1, 0));

	// place camera a bit in front of camera
	glm::vec3 pos;
	if (running) {
		pos = glm::vec3(-.03f, -.1, 0.12f);
	}
	else {
		pos = glm::vec3(0.f, -.1, 0.15f);
	}
	ret = glm::translate(ret, pos);

	// rotate camera to point in the right direction
	ret = glm::rotate(ret, .5f, glm::vec3(0, 1, 0));

	return std::make_tuple(ret, pos + flashPos);
}

void FpsCam::DrawLight(glm::vec3 position)
{
	position.y += 1.f;
	position.z += .75f;
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(1, true);
	tigl::shader->setLightPosition(0, position);
	tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.15f));
	tigl::shader->setLightDiffuse(0, glm::vec3(1.f, 1.f, 1.f));
	tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
	tigl::shader->setShinyness(1.f);
}

void FpsCam::draw() {

	std::tuple<glm::mat4, glm::vec3> tuple = PositionFlashlight();

	DrawLight(std::get<glm::vec3>(tuple));

	// set cursor.
	tigl::shader->setModelMatrix(std::get<glm::mat4>(tuple));

	// draw flashlight
	flashlight->draw();
}

// responsible for giving a limit on running.
void FpsCam::MaxRunTime()
{
	if (wPressed && shiftPressed && !running && !recovering) {
		timeStarted = clock();
		running = true;
	}
	else if ((!wPressed || !shiftPressed) && running) {
		running = false;
	}

	if (clock() - timeStarted > maxRunningTime * CLOCKS_PER_SEC && running && !recovering) {
		playingSpecialSound = true;
		outOfBreath->play();

		recovering = true;
		running = false;
		recoverTime = clock();
	}

	if (clock() - recoverTime > maxRecoverTime * CLOCKS_PER_SEC && recovering) {
		recovering = false;
	}

	if (playingSpecialSound && outOfBreath->getStatus() != sf::Sound::Playing)
		playingSpecialSound = false;
}

void FpsCam::PlayFootstep() {
	// gettings sound from tuple
	sf::Sound* sound = &std::get<sf::Sound>(footsteps[soundPosition]);

	// check if sound is playing
	if (sound->getStatus() != sf::Sound::Playing) {

		// set position of listener
		sf::Listener::setDirection(position->x, position->y, position->z);

		// get random walk sound
		soundPosition = rand() % footsteps.size();

		// if running. Play sound twice as fast.
		if (shiftPressed) {
			sound->setVolume(sound->getVolume() + 10.f);
			sound->setPitch(1.5f);
		}
		else {
			sound->setVolume(20.f);
			sound->setPitch(1.f);
		}

		// give sound the data for the sound from buffer in tuple.
		sound->setBuffer(std::get<sf::SoundBuffer>(footsteps[soundPosition]));

		// set origin from the sound
		sound->setPosition(position->x, position->y, position->z);

		// play the sound
		sound->play();
	}
}

bool FpsCam::isAtEndpoint(float tolerance) {
	//std::cout << "posX: " << position->x << ", posZ: " << position->z << ". EndpointX: " << endPoint.x << ", EndPointZ: " << endPoint.z << "\n";
	if (-position->x >= endPoint.x - (tolerance / 2) && -position->x <= endPoint.x + (tolerance / 2)) {
		return -position->z >= endPoint.z - (tolerance / 2) && -position->z <= endPoint.z + (tolerance / 2);
	}
	return false;
}

void FpsCam::changeFov(float deltaTime) {

	if (shiftPressed && wPressed) {
		if (fov < 100.f) {
			fov += 50.f * deltaTime;
		}
	}
	else {
		if (fov > 80.f) {
			fov -= 50.f * deltaTime;
		}
	}
}

void FpsCam::moveCam(GLFWwindow* window, const float& speed, float deltaTime) {
	float mult = 1;

	// multiplier for running
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !recovering) {
		mult *= 3.f;
		shiftPressed = true;
	}
	else
		shiftPressed = false;

	// left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move(0, speed, deltaTime);
	}

	// right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move(180, speed, deltaTime);
	}

	// forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		wPressed = true;
		move(90, speed * mult, deltaTime);
	}
	else
		wPressed = false;

	// backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move(-90, speed, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position->y -= speed * 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position->y += speed * 0.1f;
	}
}

void FpsCam::setEndpoint(glm::vec3 endPoint)
{
	this->endPoint = endPoint;
}

void FpsCam::setSpawnTile(Tile* tile) {
	this->tile = tile;
}