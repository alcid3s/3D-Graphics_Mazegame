#pragma once
#include "headers/components/Component.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <tuple>
#include "SFML/Audio.hpp"

class ObjModel;
class Tile;

class EnemyComponent : public Component {
public:
	EnemyComponent(std::string pathToObjModel);
	~EnemyComponent();

	virtual void update(float deltaTime) override;
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
