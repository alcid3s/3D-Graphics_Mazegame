#pragma once
#include "DrawComponent.h"
#include "tigl.h"
#include <string>

class Texture;

class HUDComponent : public DrawComponent {
	std::vector<tigl::Vertex> verts;
public:
	HUDComponent(std::string path = "resource/textures/Cam1.png");
	~HUDComponent();

	void update(float deltaTime) override;
	void draw() override;

	inline void setFov(float *fov) { this->fov = fov; }
	inline void setIsRunning(bool bIsRunning) { this->bIsRunning = bIsRunning; }
	inline void setIsMoving(bool bIsMoving) { this->bIsMoving = bIsMoving; }
private:
	Texture* texture;
	glm::mat4 mat;

	float* fov;
	bool bIsRunning = false;
	bool bIsMoving = false;
private:
	void bindHUD();
	void unbindHUD();
};