#pragma once

class Coordinate {
	int x = 0;
	int y = 0;

public:
	Coordinate() : x(0), y(0) {};
	Coordinate(int x, int y) : x(x), y(y) {};

	int getX() {
		return x;
	}
	
	int getY() {
		return y;
	}

	void setX(int newX) {
		this->x = newX;
	}

	void setY(int newY) {
		this->y = newY;
	}
};