#include "headers/enemy.h"
#include "ObjModel.h"
#include "tigl.h"
#include <iostream>
#include "Tile.h"
#include "Maze/MazeGenerator.h"

const int chanceOfSound = 600;

Enemy::Enemy(std::string path) : model(new ObjModel(path))
{
	for (int i = 1; i <= 1; i++) {
		sf::Sound* sound = new sf::Sound();
		sf::SoundBuffer* buffer = new sf::SoundBuffer();

		std::string file = "resource/sounds/enemySounds/sound" + std::to_string(i) + ".wav";

		std::cout << "load file successfull: " << (buffer->loadFromFile(file) ? "true" : "false") << "\n";
		sound->setPitch(1.f);
		sound->setVolume(20.f);
		sound->setBuffer(*buffer);
		sound->setMinDistance(.1f);
		sound->setAttenuation(0.5f);
		sounds.push_back(std::make_tuple(*sound, *buffer));
	}
}

Enemy::~Enemy()
{
	delete[] model;
}

// this function can only be called once
void Enemy::SetSpawnPoint(Tile* startTile) {

	// static so this is only set once.
	static bool isSet = false;
	if (!isSet) {
		this->tile = startTile;
		this->translate = startTile->GetPosition();
		isSet = true;
	}
}

void Enemy::draw()
{
	// set cursor
	tigl::shader->setModelMatrix(glm::translate(glm::mat4(1.0f), translate));

	// draw model on cursor
	model->draw();
}

void Enemy::update(float deltaTime)
{
	if (rand() % chanceOfSound == 0) {
		playSound();
	}
	MoveRandom(1.f, deltaTime);
}

void Enemy::MoveRandom(float speed, float deltaTime) {
	if (!tile)
		return;

	if (!moving) {
		std::vector<Tile*> neighbours = GetNeighbours(tile);

		std::cout << "size of neighbours before deletion: " << neighbours.size() << "\n";

		neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(), [](Tile* tile) {
			return tile->type != Type::Floor && tile->type != Type::Endpoint;
			}), neighbours.end());

		std::cout << "size of neighbours after deletion: " << neighbours.size() << "\n";

		if (!neighbours.empty()) {
			int randomNum = rand() % neighbours.size();
			tile = neighbours.at(randomNum);
			moving = true;
		}
	}

	else {
		//std::cout << "bearing: " << tileBearing << ", position: (" << translate.x << ", " << translate.z << "). tile position: (" << tile->GetPosition().x << ", " << tile->GetPosition().z << ")\n";
		// check which way the neighbour is.

		float distance = glm::distance(tile->GetPosition(), translate);
		if (distance < speed * deltaTime) {
			translate = tile->GetPosition();
			moving = false;
		}
		else {
			auto direction = glm::normalize(tile->GetPosition() - translate);
			translate += speed * deltaTime * direction;
		}
	}
}
void Enemy::playSound() {

	int randomSound = rand() % sounds.size();

	// gettings sound from tuple
	sf::Sound* sound = &std::get<sf::Sound>(sounds[randomSound]);

	// check if sound is playing
	if (sound->getStatus() != sf::Sound::Playing) {

		// set position of listener
		sf::Listener::setDirection(translate.x, translate.y, translate.z);

		// give sound the data for the sound from buffer in tuple.
		sound->setBuffer(std::get<sf::SoundBuffer>(sounds[randomSound]));

		// set origin from the sound
		sound->setPosition(translate.x, translate.y, translate.z);

		// play the sound
		sound->play();
	}
}
