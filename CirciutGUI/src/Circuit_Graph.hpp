#pragma once

#include <SFML/Graphics.hpp>

class Graph {

private:
	
	struct Corner {
		int item = 0;
		std::vector<int> neighbors;

		Corner(int Item) {
			item = Item;
			neighbors.reserve(8);
		}
	};
	sf::Event evnt;
	sf::RenderWindow win;
	std::vector<sf::CircleShape> allCircles;
	std::vector<sf::RectangleShape> allVertices;
	std::vector<Corner> Vector;

public:

	Graph();
	~Graph();
	
	void newItem(int serial);
	void link(int Index, int Link);

	void clearAll();
	void updateWin();

	void printCorner(const Corner& corner);
	void printGraph();


private:
	void setGraph();

};