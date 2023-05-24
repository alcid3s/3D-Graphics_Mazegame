#include "LoadingScreen.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>

LoadingScreen::LoadingScreen()
{
	texture = new Texture("resource/textures/LoadScreen.png");
	plane = new PlaneComponent(glm::vec3(1, 0, 1), glm::vec3(0, 0, 0), texture);
}

LoadingScreen::~LoadingScreen()
{

}

void LoadingScreen::update()
{
	
}

glm::mat4 LoadingScreen::GetMatrix() {
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, glm::radians(90.f), glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, glm::radians(180.f), glm::vec3(0, 1, 0));
	ret = glm::translate(ret, plane->getTranslate());
	return ret;
}

void LoadingScreen::draw()
{
	plane->draw();
}
