#pragma once

import Players;

class Scoreboard {
	int P1max = 100;
	int P2max = 100;
	
	int P1Health = 0;
	int P2Health = 0;

public:
	Scoreboard() {};
	Scoreboard(int health) : P1Health(health), P2Health(health) {};

	void setHealth(int health) {
		P1Health = health;
		P2Health = health;
	}

	//Decrement enemy health on hit
	bool decrementHealth(Players player) {
		if (player == Player1) {
			P2Health--;
			return P2Health == 0;
		}
		else {
			P1Health--;
			return P1Health == 0;
		}
	}

	//Decrement points on miss
	void decrementPoints(Players player) {
		if (player == Player1) {
			P1max--;
		}
		else {
			P2max--;
		}
	}

	int getPointsFor(Players player) {
		if (player == Player1) {
			return P1max;
		}
		else {
			return P2max;
		}
	}

};