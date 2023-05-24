#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <string>
#include <vector>
#include <tuple>
#include "SFML/Audio.hpp"
#include "enumType.h"

class ObjModel;
class Tile;

class Enemy {
public:
	Enemy(std::string path);
	~Enemy();

	void draw();
	void update(float deltaTime);

	void SetSpawnPoint(Tile* startTile);
private:
	// 3d model for monster
	ObjModel* model = nullptr;

	// sounds monster randomly makes
	std::vector<std::tuple<sf::Sound, sf::SoundBuffer>> sounds;

	// holds current tile;
	Tile* tile;

	// holds current position.
	glm::vec3 translate = glm::vec3(0, 0, 0);

	// currently in moving transition
	bool moving = false;

private:
	void playSound();
	void MoveRandom(float speed, float deltaTime);
};