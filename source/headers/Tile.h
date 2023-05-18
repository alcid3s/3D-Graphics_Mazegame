#pragma once
#include <glm/gtc/matrix_transform.hpp>
enum Type;
class Gameobject;

class Tile {
public:
	Tile(Gameobject *, Type type, glm::vec3 position, bool visited);
	~Tile();

	void draw();

	void setGameobject(Gameobject*);

	bool visited;
	Type type;
	inline glm::vec3 GetPosition() const { return position; }
private:
	glm::vec3 position;

	Gameobject* gameobject;
};