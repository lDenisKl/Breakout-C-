#pragma once
#include "GameObject.h"
#include "Point.h"

class Ball : public GameObject { // Шар
private:
	float radius = 10.0f; 
	float speedX = 0.0f;	  // Скорость шара
	float speedY = 0.0f;
	float velocity = 0.0f;
	int   power = 1.0f;	      // Кол-во жизней, забираемых у плашки при столкновении 
public:
	Ball(float x,float y,float radius,float velo) {
		this->transform = Point(x, y);
		this->radius = radius;
		velocity = velo;
	}
	Point getSpeed() const { return Point(speedX, speedY); }
	void setSpeed(Point speedVec) { speedX = speedVec.x; speedY = speedVec.y; }
	float getRadius() const { return radius; }
	float getPower() const { return power; }
	float getVelocity() const { return velocity; }
};
