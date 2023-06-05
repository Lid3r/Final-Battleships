#pragma once
#include "Field.h"
#include <latch>
#include <thread>
#include <vector>

import Positions;

std::latch threadLatch(10);

class GameBoard {
	Field board[10][10];


public:
	GameBoard() {
		std::vector<std::thread> threads;

		for (int x = 0; x < 10; x++) {
			threads.emplace_back(&GameBoard::setRow, this, x);
		}

		for (auto& thread : threads) {
			thread.join();
		}

	};

	void setRow(int x) {
		for (int y = 0; y < 10; y++) {
			board[x][y].setCoord(x, y);
		}
		threadLatch.arrive_and_wait();
	}

	//See if setting ships is possible.
	//Return false if ship gets out of bounds or there is already a ship on the path
	//Return true otherwise
	bool canSetShip(Coordinate coord, Players player, Ship* ship) {
		Coordinate begin = ship->getStart();
		Coordinate end = ship->getEnd();


		if (begin.getX() < 0 || end.getX() < 0 || begin.getX() > 10 || end.getX() > 10) {
			return false;
		}

		//Vertical alignment, iterate only on y axis
		if (begin.getX() == end.getX()) {
			int start = std::min(begin.getY(), end.getY());
			for (int y = start; y < start + ship->getLength(); y++) {
				if (board[coord.getX()][y].hasShip(player)) {
					return false;
				}
			}
		}
		else {
			//Horizontal, iterate only on x axis
			int start = std::min(begin.getX(), end.getX());
			for (int x = start; x < start + ship->getLength(); x++) {
				if (board[x][coord.getY()].hasShip(player)) {
					return false;
				}
			}
		}

		return true;
	}

	void setShip(Coordinate coord, Players player, Ship* ship) {
		Coordinate begin = ship->getStart();
		Coordinate end = ship->getEnd();

		//Vertical alignment, iterate only on y axis
		if (begin.getX() == end.getX()) {
			int start = std::min(begin.getY(), end.getY());
			for (int y = start; y < start + ship->getLength(); y++) {
				board[coord.getX()][y].setShip(ship, player);
			}
		}
		else {
		//Horizontal, iterate only on x axis
			int start = std::min(begin.getX(), end.getX());
			for (int x = start; x < start + ship->getLength(); x++) {
				board[x][coord.getY()].setShip(ship, player);
			}
		}
	}

	int shoot(Coordinate coord, Players player) {
		if (board[coord.getX()][coord.getY()].canBeHit(player)) {
			return board[coord.getX()][coord.getY()].hitShip(player);
		}
		else {
			return 2;
		}
	}

	int getState(int x, int y, Players player) {
		return board[x][y].getState(player);
	}

	sf::Color getCircleColor(int x, int y, Players player, bool isPrep, bool showYours) {
		return board[x][y].getCircleColor(player, isPrep, showYours);
	}
};