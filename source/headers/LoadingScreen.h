#pragma once
#include "headers/components/PlaneComponent.h"

class Texture;
class LoadingScreen {
public:
	LoadingScreen();
	~LoadingScreen();

	void update();
	void draw();

	glm::mat4 GetMatrix();

private:
	PlaneComponent* plane;
	Texture* texture;
};