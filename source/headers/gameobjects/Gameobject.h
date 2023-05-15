#pragma once

class Gameobject {
public:
	Gameobject();
	~Gameobject();

	virtual  void update() = 0;
	virtual void draw() = 0;
private:

};