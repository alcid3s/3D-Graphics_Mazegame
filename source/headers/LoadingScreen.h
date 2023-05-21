#pragma once
#include "Plane.h"

class Texture;
class LoadingScreen {
public:
	LoadingScreen();
	~LoadingScreen();

	void update();
	void draw();

	glm::mat4 GetMatrix();

	inline glm::vec3 GetTransform() const { return plane->GetTransform(); }
	inline glm::vec3 GetRotation() const { return plane->GetRotation(); }
	inline glm::vec3 GetPosition() const { return plane->GetTranslate(); }
private:
	Plane* plane;
	Texture* texture;
};