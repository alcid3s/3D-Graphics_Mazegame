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

	float* fov;
	float previousFov = 0.f;

	float totalDifference = 0.f;

	bool bIsRunning = false;
	bool bIsMoving = false;
private:
	void bindHUD();
	void unbindHUD();

	void updateHUD();
	float mapValue(float value);
};