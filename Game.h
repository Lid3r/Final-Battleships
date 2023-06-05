#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ranges>
#include <numeric>
#include <Windows.h>
#include <regex>
#include <fstream>
#include <filesystem>
#include "GameBoard.h"
#include "Scoreboard.h"
using namespace std;
using namespace sf;

namespace fs = std::filesystem;

import Utilities;

class Game
{
	double height, width;
	string fontPath, mapPath, mapInfoPath;
	Font font;
	Sprite map;
	Sprite mapInfo;

	GameBoard board;

	Scoreboard score;

	double cellWidth;
	double cellHeight;

	vector<int> shipLengths;
	//const int shipLengths[3] = {2,3,4};

	sf::Color bgColor = sf::Color(217, 242, 255);
public:
	Game(string fontPath, string mapPath, string mapInfoPath, vector<int>& ships, int windowHeight, int windowWidth) : height(windowHeight), width(windowWidth), mapInfoPath(mapInfoPath), fontPath(fontPath), mapPath(mapPath), shipLengths(ships) {
		font.loadFromFile(fontPath);

		auto numbers = views::all(shipLengths) | views::transform([](int x) {return x; });
		int health = std::reduce(numbers.begin(), numbers.end());

		score.setHealth(health);
	};

	void menu() {
		RenderWindow window(VideoMode(width, height, 32), "BATTLESHIPS", Style::Default);
		Text title("Battleships", font, Text::Bold);
		Text options[2];
		string str[] = { "Play" , "Exit" };

		title.setCharacterSize(60);
		title.setFillColor(sf::Color(53, 89, 104));
		title.setPosition(width / 2 - title.getLocalBounds().width / 2, 100);

		for (int i = 0; i < 2; i++)
		{
			options[i].setFont(font);
			options[i].setCharacterSize(50);
			options[i].setString(str[i]);
			options[i].setPosition(width / 2 - options[i].getLocalBounds().width / 2, 200 + i * 100);

		}

		bool relinquishControl = false;

		while (window.isOpen() && !relinquishControl) {
			Event event;
			Vector2f mouse(Mouse::getPosition(window));
			
			while (window.pollEvent(event)) {
				//Exit was clicked or window was closed
				if (event.type == Event::Closed || (options[1].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased)) {
					window.close();
				}

				//Play was clicked
				if (options[0].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased) {
					relinquishControl = true;
				}

				window.clear(bgColor);

				for (int i = 0; i < 2; i++) {
					if (options[i].getGlobalBounds().contains(mouse)) {
						options[i].setFillColor(sf::Color(53, 89, 104));
					}
					else {
						options[i].setFillColor(sf::Color(159, 210, 239));
					}
				}

				window.draw(title);

				for (int i = 0; i < 2; i++)
				{
					window.draw(options[i]);
				}
				window.display();
			}

		}

		if (relinquishControl) {
			play(window);
		}
	}


	void drawBoard(CircleShape display[][10], Players currentPlayer, bool isPrep, bool showYours) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				display[j][i] = CircleShape(cellWidth / 2);
				display[j][i].setFillColor(board.getCircleColor(i, j, currentPlayer, isPrep, showYours));
				display[j][i].setPosition(50 + cellWidth + i * cellWidth + cellWidth - display[j][i].getLocalBounds().width, 55 + cellHeight + j * cellHeight + cellHeight - display[j][i].getLocalBounds().height - 5);
			}
		}
	}

	void drawPlayer(Players player, sf::RenderWindow &window) {
		string playerName = player == Player1 ? "Player 1" : "Player 2";
		sf::Text displayName(playerName, font, sf::Text::Bold);
		displayName.setCharacterSize(40);
		displayName.setPosition( this->width / 2 - displayName.getLocalBounds().width / 2, 0 );
		displayName.setFillColor(sf::Color(53, 89, 104));
		window.draw(displayName);
		
	}

	void play(RenderWindow& window) {
		Texture mapTexture, mapInfoTexture;
		mapTexture.loadFromFile(mapPath);
		mapInfoTexture.loadFromFile(mapInfoPath);

		mapInfo.setTexture(mapInfoTexture);
		mapInfo.setScale(0.3, 0.3);
		mapInfo.setPosition(50, 50);

		map.setTexture(mapTexture);
		map.setScale(0.3, 0.3);
		this->cellWidth = map.getGlobalBounds().width / 10;
		this->cellHeight = map.getGlobalBounds().height / 10;
		map.setPosition(50 + cellWidth, 50 + cellHeight);


		CircleShape display[10][10];

		RectangleShape rectangle;
		rectangle.setOutlineColor(Color::Black);
		rectangle.setOutlineThickness(2);
		rectangle.setFillColor(Color::Transparent);


		vector<Ship*> ships;
		
		int counter = 0;
		Players currentPlayer = Player1;
		bool isPrep = true;
		Position rotation(0);
		
		//Flag to know if there is a point to saving a score if the game was exited before ending
		bool finishedProperly = false;
		
		while (window.isOpen() && isPrep) {
			Event event;
			Vector2f mouse(Mouse::getPosition(window));

			rectangle.setSize(Vector2f(shipLengths[counter] * cellWidth, cellHeight));

			utils::setRectanglePosition(rectangle, mouse, cellWidth, cellHeight);

			while (window.pollEvent(event)) {
				//Exit was clicked or window was closed
				if (event.type == Event::Closed) {
					window.close();
				}

				drawBoard(display, currentPlayer, isPrep, true);

				//In the event of a click, the ships will be set
				setShips(event, mouse, display, currentPlayer, counter, shipLengths[counter], ships, rotation, rectangle);



				window.clear(this->bgColor);
				window.draw(mapInfo);
				window.draw(map);
				drawPlayer(currentPlayer, window);

				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						window.draw(display[j][i]);
					}
				}
				window.draw(rectangle);
				window.display();

				//Is it shooty time :D
				if (counter == shipLengths.size() && currentPlayer == Player2) {
					currentPlayer = Player1;
					counter = 0;
					isPrep = false;
				}
			}
		}

		//Shooting phase
		bool isWin = false;
		


		while (window.isOpen() && !isWin) {
			Event event;
			Vector2f mouse(Mouse::getPosition(window));
		
			while (window.pollEvent(event)) {
				drawBoard(display, currentPlayer, isPrep, false);

				if (event.type == Event::Closed) {
					window.close();
				}


				if (event.type == Event::MouseButtonReleased && map.getGlobalBounds().contains(mouse)) {
					int x = (mouse.x - map.getGlobalBounds().left) / (map.getGlobalBounds().width / 10);
					int y = (mouse.y - map.getGlobalBounds().top) / (map.getGlobalBounds().height / 10);

					currentPlayer = static_cast<Players>(counter % 2);

					int result = board.shoot(Coordinate(x, y), currentPlayer);
					bool isDed = false;

					if (result == 0) {
						//miss
						score.decrementPoints(currentPlayer);
						counter++;
						currentPlayer = static_cast<Players>(counter % 2);

						utils::createMessageBox("You missed :c", font, 800);
					}
					else if (result == 1) {
						//hit
						isDed = score.decrementHealth(currentPlayer);
						if (isDed) {
							isWin = true;
							finishedProperly = true;
						}
						//Don't increment, player gets another shot
					}
					else {
						//cannot be hit
					}
				}

				window.clear(Color(217, 242, 255));
				window.draw(map);
				window.draw(mapInfo);
				drawPlayer(currentPlayer, window);

				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						window.draw(display[j][i]);
					}
				}

				window.display();
			}

		}
		

		//This is the end, my only friend
		if (finishedProperly) {
			getNickAndSaveScore(currentPlayer);
		}

		//Yeet the ships at the end
		for (auto& ship : ships) {
			delete ship;
		}
	}

	void getNickAndSaveScore(Players currentPlayer) {
		sf::RenderWindow nameBox(sf::VideoMode(800, 200), "Input your nick");
		sf::Text info("Input your nick (3 letters or numbers)", font);
		info.setCharacterSize(35);
		info.setFillColor(sf::Color(53, 89, 104));
		info.setPosition(400 - info.getLocalBounds().width / 2, 0);
		nameBox.clear(this->bgColor);
		nameBox.draw(info);
		nameBox.display();

		string nick;
		regex pattern("[A-Z0-9]{3}");


		while (nameBox.isOpen()) {
			sf::Event event;
			while (nameBox.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					nameBox.close();
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
					if (utils::isCorrect(nick, pattern)) {
						nameBox.close();
					}
					else {
						continue;
					}
				}

				sf::Text nickToDisplay(nick, font);
				nickToDisplay.setCharacterSize(30);
				nickToDisplay.setFillColor(sf::Color(53, 89, 104));
				nickToDisplay.setPosition(400 - nickToDisplay.getLocalBounds().width / 2, 100);

				if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode == '\b' && nick.length() != 0) {
						nick.pop_back();
					}
					else {
						if (event.text.unicode < 128 && nick.length() != 3) {
							//Prevent newlines by enter
							if (event.text.unicode == 8 || event.text.unicode == 13) {
								continue;
							}
							else {
								nick.push_back(toupper(static_cast<char>(event.text.unicode)));
							}
						}
					}

				}
				nameBox.clear(this->bgColor);
				nameBox.draw(info);
				nameBox.draw(nickToDisplay);
				nameBox.display();

			}
		}

		//Save the score

		//If there is no directory for a score, create one
		if (!fs::is_directory("scores")) {
			fs::create_directory("scores");
		}

		//Save only if correct (to override exiting prematurely)
		if (utils::isCorrect(nick, pattern)) {
			ofstream toSave("scores/" + nick + ".txt");
			if (toSave.is_open()) {
				toSave << this->score.getPointsFor(currentPlayer);
				toSave.close();
			}
		}
	}

	void setShips(Event event, Vector2f mouse, CircleShape display[][10], Players &currentPlayer, int& counter, int shipSize, vector<Ship*> &ships, Position &rotation, RectangleShape &rectangle) {
		if (event.type == Event::KeyReleased && event.key.code == sf::Keyboard::R) {
			rotation.rotate();
			rectangle.rotate(90);
			utils::setRectanglePosition(rectangle, mouse, cellWidth, cellHeight);
		}


		if (event.type == Event::MouseButtonReleased && map.getGlobalBounds().contains(mouse)) {
			int x = (mouse.x - map.getGlobalBounds().left) / (map.getGlobalBounds().width / 10);
			int y = (mouse.y - map.getGlobalBounds().top) / (map.getGlobalBounds().height / 10);

			//Setting ships
			Ship* ship = new Ship(Coordinate(x, y), rotation.getPosition(), shipSize);

			//Check if position and rotation is allowed, if yes, set ship
			if (board.canSetShip(Coordinate(x, y), currentPlayer, ship)) {
				board.setShip(Coordinate(x, y), currentPlayer, ship);
				ships.push_back(ship);
				counter++;
			}
			else {
				delete ship;
			}
			
			if (counter == this->shipLengths.size() && currentPlayer == Player1) {
				counter = 0;
				currentPlayer = Player2;
			}
		}
	}


};
