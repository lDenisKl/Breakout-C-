#pragma once
#include "GameObject.h"

class Ball : public GameObject { // Шар
private:
	float radius = 10.0f; 
	float speed = 10.0f;	  // Скорость шара
	int   power = 1.0f;	      // Кол-во жизней, забираемых у плашки при столкновении 

};