#include "Paddle.h"
#include <string>

class Plate : public Paddle {   // Плашка для уничтожения
private:
	int			lifes;			// кол-во ударов для уничтожения
	std::string feature;		// суперспособность при уничтожении плашки
};