#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

import Utilities;

class FieldState : public State {
	int state = 0;

public:
	FieldState() {};
	FieldState(int state) : state(state) {};

	int getState() {
		return state;
	}

	void setState(int state) {
		this->state = state;
	}

	sf::Color getColorForField(bool isPrepPhase) {
		switch (state)
		{
		case 0:
			//Empty
			return sf::Color::Transparent;
			break;
		case 1:
			//Has ship, but we don't show them
			return isPrepPhase ? sf::Color(150, 255, 129) : sf::Color::Transparent;
			break;
		case 2:
			//Ship hit
			return sf::Color::Yellow;
			break;
		case 3:
			//Ship destroyed
			return sf::Color::Red;
			break;
		case 4:
			//Hit but no ship
			return sf::Color(0,0,0);
			break;
		
		default:
			//Should never happen but just to be safe
			return sf::Color::Transparent;
			break;
		}
	}
};