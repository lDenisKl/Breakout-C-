#pragma once
#include "GameObject.h"

class Paddle : public GameObject { // Плитка игрока
private:
	float speed;			// скорость движения плитки
	float width; 
	float height;
public:
	Paddle() {};
	Paddle(float speed,float width,float height) {
		this->transform = Point(0,0);
		
		this->speed = speed;
		this->width = width;
		this->height = height;
	}
	float getWidth() const { return width; }
	float getHeight() const { return height; }

};