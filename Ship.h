#pragma once
#include "Coordinate.h"
#include "Position.h"
#include <string>

import Positions;
import Utilities;

class Ship {
protected:
	Coordinate start;
	Coordinate end;
	Position position;
	int length = 0;
	int startLength = 0;
	int state = 1;

public:
	Ship() : start(Coordinate(0, 0)), end(Coordinate(0, 0)) {};
	Ship(Coordinate start, int position, int length) : start(start), length(length), startLength(length), position(Position(position)) {
		switch (position)
		{
		case Up:
			end.setX(start.getX());
			end.setY(start.getY() - length+1);
			break;
		case Right:
			end.setX(start.getX() + length);
			end.setY(start.getY());
			break;
		case Down:
			end.setX(start.getX());
			end.setY(start.getY() + length);
			break;
		case Left:
			end.setX(start.getX() - length+1);
			end.setY(start.getY());
			break;

		default:
			break;
		}
	};

	Position getPosition() {
		return position;
	}

	Coordinate getStart() {
		return start;
	}
	
	Coordinate getEnd() {
		return end;
	}

	int getLength() {
		return length;
	}

	bool isSunk() {
		return length == 0;
	};

	bool isHit() {
		return length != startLength;
	}

	bool hitShip() {
		length--;
		if (isSunk()) {
			state = utils::FieldStateEnum::Destroyed;
		}
		return isSunk();
	}

	int getState() {
		return state;
	}
};