#pragma once
#include "DrawComponent.h"
#include <string>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class ObjModel;
class GameObject;
//class GLFWwindow;

class AltarComponent : public DrawComponent {
public:
	AltarComponent(GLFWwindow* window, std::shared_ptr<GameObject> obj, std::string path = "resource/models/altar/Altar.obj");
	~AltarComponent();

	void update(float deltaTime) override;
	void draw() override;

	inline bool endReached() { return bReachedEnd; }
private:
	const float minimalDistance = .5f;
	bool bReachedEnd = false;
	std::shared_ptr<GameObject> player;
	ObjModel* model;
	GLFWwindow* window;
};