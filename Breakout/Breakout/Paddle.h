#pragma once
#include "GameObject.h"

class Paddle : public GameObject { // Плитка игрока
private:
	float speed;			// скорость движения плитки
	
float getTouchOffset(float touchX) {
	float diff = (transform.x + width) - touchX;
	float offset = width - diff;
	float result = 2 * offset / width;
	return result - 1;
}
protected:
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

	void setWidth(float nwidth) { width = nwidth; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }

};