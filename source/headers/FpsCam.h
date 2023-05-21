#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include "SFML/Audio.hpp"
struct GLFWwindow;

class Tile;

class FpsCam {
public:
	FpsCam(GLFWwindow*);

	glm::mat4 getMatrix();
	void update(GLFWwindow*, float deltaTime);
	void setEndpoint(glm::vec3);
	void setSpawnTile(Tile* tile);

	inline float GetFov() const { return fov; }
	bool endPointReached;

	glm::vec3* position;

	// if player is making a sound
	bool playingSpecialSound = false;

	// variable for indicating when running
	bool running;
private:

	// current fov.
	float fov;

	// variables for maximum runtime and fov.
	bool recovering;
	bool shiftPressed;
	bool wPressed;
	clock_t timeStarted = 0;
	clock_t recoverTime = 0;
	sf::Sound* outOfBreath = nullptr;

	// random numbers between 0 and the amount of sounds available for footsteps so footstep sounds are randomised and not repetitive.
	int soundPosition, i = 0;

	// current tile position
	Tile* tile;

	// used to determine if player reached the endpoint
	glm::vec3 endPoint;

	// rotation of camera when starting.
	glm::vec2 rotation = glm::vec2(0, 150);

	// used for collision
	bool closeToEdge = false;
	std::vector<Tile*> neighbours;

private:
	void move(float angle, float fac, float deltaTime);
	void moveCam(GLFWwindow* window, const float& speed, float deltaTime);
	void changeFov(float deltaTime);
	bool isAtEndpoint(float tolerance);
	void PlayFootstep();
	void MaxRunTime();

	std::vector<std::tuple<sf::Sound, sf::SoundBuffer>> footsteps;
};