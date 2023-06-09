#pragma once
#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"

class GLFWwindow;

enum MenuType {
	MainMenu= 0,
	Loading = 1,
	Options = 2,
	Playing = 3,
	None = -1
};

class GuiManager {
public:
	GuiManager(GLFWwindow* window, const int& x, const int& y);
	~GuiManager();

	void update();
	void draw();

	void init();

	MenuType menuType;
private:
	GLFWwindow* window;
	const int& screenX, screenY;
	bool bShutdownGui = false;
	
private:
	void drawLoadingScreen();
	void drawMainMenu();
	void setColorGui();
	void shutdownImGui();
};
