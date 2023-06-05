#pragma once

class Position {
	int position;

public:
	Position() : position(0) {};
	Position(int position) : position(position) {};

	int getPosition() {
		return position;
	}

	void rotate() {
		position = (position + 1) % 4;
	}

	void setPosition(int position) {
		this->position = position;
	}
};