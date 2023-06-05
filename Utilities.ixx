#include <SFML/Graphics.hpp>
#include <string>
#include <Windows.h>
#include <regex>
export module Utilities;
export namespace utils {

	export void setRectanglePosition(sf::RectangleShape &rectangle, sf::Vector2f mouse, double cellWidth, double cellHeight) {
	
		switch (static_cast<int>(rectangle.getRotation()))
		{
		case 0:
			rectangle.setPosition(mouse.x - cellWidth / 2, mouse.y - cellHeight / 2);
			break;
		case 90:
			rectangle.setPosition(mouse.x + cellWidth / 2, mouse.y - cellHeight / 2);
			break;
		case 180:
			rectangle.setPosition(mouse.x + cellWidth / 2, mouse.y + cellHeight / 2);
			break;
		case 270:
			rectangle.setPosition(mouse.x - cellWidth / 2, mouse.y + cellHeight / 2);
			break;
		default:
			break;
		}
	};

	export void createMessageBox(std::string message, sf::Font font, int TTL) {
		sf::RenderWindow messageBox(sf::VideoMode(200, 100), message);
		sf::Text info(message, font, sf::Text::Bold);
		info.setCharacterSize(20);
		info.setFillColor(sf::Color::Black);
		info.setPosition(100 - info.getLocalBounds().width / 2, 30);

		messageBox.clear(sf::Color::White);
		messageBox.draw(info);
		messageBox.display();

		while (messageBox.isOpen()) {
			Sleep(TTL);
			messageBox.close();
		}
	};

	export bool isCorrect(std::string nick, std::regex pattern) {
		return std::regex_match(nick, pattern);
	}

	export enum FieldStateEnum {
		Empty = 0,
		Filled = 1,
		Hit = 2,
		Destroyed = 3,
		HitEmpty = 4
	};

}