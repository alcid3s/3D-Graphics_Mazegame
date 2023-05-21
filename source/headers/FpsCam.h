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
private:
	float fov;
	bool shiftPressed;

	int soundPosition, i = 0;

	Tile* tile;

	glm::vec3 endPoint;
	glm::vec2 rotation = glm::vec2(0, 180);

	bool isJumping;


	bool closeToEdge = false;
	std::vector<Tile*> neighbours;

private:
	void move(float angle, float fac, float deltaTime);
	void moveCam(GLFWwindow* window, const float& speed, float deltaTime);
	void changeFov(float deltaTime);
	bool isAtEndpoint(float tolerance);
	void PlayFootstep();

	std::vector<std::tuple<sf::Sound, sf::SoundBuffer>> footsteps;
};