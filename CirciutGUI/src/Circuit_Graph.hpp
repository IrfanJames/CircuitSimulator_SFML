#pragma once

#include <SFML/Graphics.hpp>

class Graph {

	struct Corner {
		int item = 0;
		std::vector<int> neighbors;

		Corner(int Item) {
			item = Item;
			neighbors.reserve(8);
		}
	};
	
public:
	Graph();
	~Graph();
	
	void newItem(int serial);
	void newItem_noSetGUI(int serial);
	void link(int corner1, int corner2);
	void link_noSetGUI(int corner1, int corner2);
	void clearAll();

	void updateWin();
	void createWindow();

	void printCorner(const Corner& corner);
	void printGraph();

	void setGraph();

private:
	int draggingCircle = -1;
	sf::Vector2f mouseOffSet;
	sf::Vector2f mouseHold;
	sf::Vector2f viewCenter;
	sf::Event evnt;
	sf::RenderWindow win;

	std::vector<sf::CircleShape> allCircles;
	std::vector<sf::RectangleShape> allVertices;
	std::vector<Corner> Vector;
};

