#pragma once
#include <glm/gtc/matrix_transform.hpp>
enum Type;
class Gameobject;
class ObjModel;

class Tile {
public:
	Tile(Gameobject *, Type type, glm::vec3 position, bool visited);
	~Tile();

	void draw();

	void setGameobject(Gameobject*);
	void setModel(ObjModel*);

	bool visited;
	Type type;
	inline glm::vec3 GetPosition() const { return position; }
private:
	ObjModel* model = nullptr;
	glm::vec3 position;

	Gameobject* gameobject;
};