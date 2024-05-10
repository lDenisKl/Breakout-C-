#include "GameObject.h"

class Ball : GameObject { // Шар
private:
	float radius; 
	float speed;	      // Скорость шара
	int   power;	      // Кол-во жизней, забираемых у плашки при столкновении 
};