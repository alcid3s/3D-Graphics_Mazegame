#include "FpsCam.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Tile.h"
#include "enumType.h"
#include "Maze/MazeGenerator.h"

#include <iostream>
FpsCam::FpsCam(GLFWwindow* window) : fov(80.f), shiftPressed(false), endPointReached(false), isJumping(false) {
	srand(time(NULL));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	for (int i = 1; i <= 7; i++) {
		sf::Sound* sound = new sf::Sound();;
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
}

glm::mat4 FpsCam::getMatrix() {
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, *position);
	return ret;
}

bool closeToEdge = false;
std::vector<Tile*> neighbours;
void FpsCam::move(float angle, float fac, float deltaTime) {
	if (!tile)
		return;

	const float edge = 0.5f;
	const float tolerance = 0.1f;

	float x = tile->GetPosition().x;
	float z = tile->GetPosition().z;

	float movementX = position->x + ((float)cos(rotation.y + glm::radians(angle)) * -fac) * deltaTime;
	float movementZ = position->z + ((float)sin(rotation.y + glm::radians(angle)) * -fac) * deltaTime;

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

	// player is close to edge. Check if it crossed the tile
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
			else if(neighbours.at(i)->type != Type::Floor) {
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

	if (shiftPressed) {
		if (fov < 100.f) {
			fov -= 50.f * deltaTime;
		}
	}
	else {
		if (fov > 80.f) {
			fov += 50.f * deltaTime;
		}
	}
}

void FpsCam::moveCam(GLFWwindow* window, const float& speed, float deltaTime) {
	float mult = 1;

	// multiplier for running
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
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
		move(90, speed * mult, deltaTime);
	}

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