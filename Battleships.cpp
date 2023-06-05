#include <SFML/Graphics.hpp>
#include <string>
#include "Game.h"
#include "json.hpp"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
using namespace sf;
using namespace std;
using json = nlohmann::json;

int main()
{

	ifstream input("config.json");
	if (!input.is_open()) {
		cout << "Config file not found!" << endl;
		return -1;
	}

	json config = json::parse(input);
	input.close();

	string fontPath = config.value("font", "");
	string mapPath = config.value("map", "");
	string mapInfoPath = config.value("mapInfo", "");
	vector<int> ships = config["ships"].get<vector<int>>();


	if (fontPath == "" || mapPath == "" || mapInfoPath == "" || ships.size() == 0) {
		cout << "Config file incorrect!" << endl;
		return -1;
	}

	Game game(fontPath, mapPath, mapInfoPath, ships, WINDOW_HEIGHT, WINDOW_WIDTH);

	game.menu();

	return 0;
}
