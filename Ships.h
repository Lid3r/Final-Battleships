#pragma once
#include "Ship.h"

import Positions;

class Destroyer : public Ship {
	int length = 2;
	int remaining = 2;
public:
	Destroyer(int startX, int startY, int endX, int endY, int position, char symbol) : Ship(Coordinate(startX, startY), Coordinate(endX, endY), position, symbol) {};

	bool isSunk() {
		return remaining == 0;
	}
};