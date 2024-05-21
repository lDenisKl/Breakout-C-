#pragma once

struct Point {
public:
	float x;
	float y;
	Point(){}
	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}
};
bool operator==(Point& a, Point& b) {
	return (a.x == b.x && a.y == b.y) ? true : false;
}