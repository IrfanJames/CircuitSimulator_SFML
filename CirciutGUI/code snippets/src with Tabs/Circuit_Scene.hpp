#pragma once

#include "SFML/Graphics.hpp"
#include "Circuit_Entity.hpp"

class Scene {
public:
	bool isOpen = true;
	std::string path = "N/A";
	sf::RectangleShape allSqr;

	sf::View view;

	/*Grid*/
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;

	/*Drag*/
	float viewX = 0, viewY = 0;
	float verX = 0, verY = 0;
	float horX = 0, horY = 0;
	int verBrightCount = 5, horBrightCount = 5;

	/*Vectors*/
	std::vector<Entity> comp;
	std::vector<int> virSerial;
	std::vector<sf::Sprite> virSprite;
	std::vector<int> virSerialShift;
	std::vector<sf::Vector2f> allEnds;
	std::vector<sf::CircleShape> allEndCircles;
public:
	Scene(std::string Path = "N/A") {
		static int untitled = 0;

		if (Path == "N/A") Path += std::to_string(untitled++);

		path = Path;
	}

	std::string getName() {
		if (3 <= path.size() && path[0] == 'N' && path[1] == '/' && path[2] == 'A') {
			return "Untitled-" + path.substr(3);
		}
		else {
			int off = path.size() - 1;
			while (0 < off && path.size() > --off && path[off] != '\\');
			if (path.find('\\') != std::string::npos) off++;
			off = (0 <= off) ? off : 0;
			
			int count = 0;
			while (path.size() > off + count && path[off + count++] != '.');
			if (path.find('.') != std::string::npos) count--;

			//std::cout << "\n" << "(" << path[off] << ", " << path[off + count - 1] << ") " << path.substr(off, count);

			return path.substr(off, count);
		}
	}

	~Scene() { ; }
};