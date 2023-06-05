#pragma once
#include "Coordinate.h"
#include "Ship.h"
#include "FieldState.h"
#include <iostream>

import Players;
import Utilities;

class Field {
	Coordinate coordinate;

	//Player1
	FieldState state1;
	Ship* ship1 = nullptr;

	//Player2
	FieldState state2;
	Ship* ship2 = nullptr;

public:
	Field() {};
	Field(Coordinate coord): coordinate(coord) {};

	void setCoord(int x, int y) {
		coordinate.setX(x);
		coordinate.setY(y);
	}

	//Set ship at a given location, assigning the state of filled
	void setShip(Ship* ship, Players player) {
		if (player == Player1) {
			if (this->ship1 != nullptr) {
				//throw exception
			}
			else {
				this->ship1 = ship;
				state1.setState(utils::FieldStateEnum::Filled);
			}
		}
		else {
			if (this->ship2 != nullptr) {
				//throw exception
			}
			else {
				this->ship2 = ship;
				state2.setState(utils::FieldStateEnum::Filled);
			}
		}
	}

	bool hasShip(Players player) {
		if (player == Player1) {
			return ship1 != nullptr;
		}
		else {
			return ship2 != nullptr;
		}
	}

	bool hasEnemyShip(Players player) {
		if (player == Player1) {
			return ship2 != nullptr;
		}
		else {
			return ship1 != nullptr;
		}
	}

	bool canBeHit(Players player) {
		if (player == Player1) {
			return (
				state2.getState() == utils::FieldStateEnum::Filled ||
				state2.getState() == utils::FieldStateEnum::Empty
				);
		}
		else {
			return (
				state1.getState() == utils::FieldStateEnum::Filled ||
				state1.getState() == utils::FieldStateEnum::Empty
				);
		}
	}

	//If there is a ship, mark the field as hit and decrement the ships length (health)
	bool hitShip(Players player) {
		if (hasEnemyShip(player)) {

			if (player == Player1) {
				ship2->hitShip();
				state2 = utils::FieldStateEnum::Hit;
			}
			else {
				ship1->hitShip();
				state1 = utils::FieldStateEnum::Hit;
			}
			
			updateState(player);
			return true;
		}
		else {
			if (player == Player1) {
				state2 = utils::FieldStateEnum::HitEmpty;
			}
			else {
				state1 = utils::FieldStateEnum::HitEmpty;
		}
		return false;
	}}

	//Since the fields aren't synced with ships (don't know when the ship is sunk) this updates the fields if the ships is sunk
	void updateState(Players player) {
		if (!hasEnemyShip(player)) {
			return;
		}

		if (player == Player1) {
			if (ship2->isSunk()) {
				state2 = utils::FieldStateEnum::Destroyed;
			}
		}
		else {
			if (ship1->isSunk()) {
				state1 = utils::FieldStateEnum::Destroyed;
			}
		}
	}

	int getState(Players player) {
		if (player == Player1) {
			return state1.getState();
		}
		else {
			return state2.getState();
		}
	}

	sf::Color getCircleColor(Players player, bool isPrep, bool showYours) {
		updateState(player);
		if (showYours) {
			if (player == Player1) {
				return state1.getColorForField(isPrep);
			}
			else {
				return state2.getColorForField(isPrep);
			}
		}
		else {
			if (player == Player1) {
				return state2.getColorForField(isPrep);
			}
			else {
				return state1.getColorForField(isPrep);
			}
		}

	}
};