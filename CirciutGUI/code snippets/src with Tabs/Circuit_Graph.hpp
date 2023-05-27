#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Circuit_Global.hpp"

class Graph {
private:
	static struct Corner {
		int item = 0;
		std::vector<Corner*> neighbors;

		Corner(int Item) {
			item = Item;
			neighbors.reserve(8);
		}
	};
	sf::RenderWindow win;
	std::vector<sf::CircleShape> allCircles;
	std::vector<sf::RectangleShape> allVertices;
	std::vector<Corner> Vector;

private:
	void setGraph() {
		win.setVisible(1);

		allCircles.clear();
		allVertices.clear();
		win.clear(sf::Color(0, 0, 0));

		/*Edges*/
		allCircles.reserve(Vector.size());
		for (int c = 0; c < Vector.size(); c++) {
			allCircles.emplace_back(30, 30);
			allCircles.back().setOrigin(30, 30);
			allCircles.back().setOutlineThickness(3);
			allCircles.back().setFillColor(sf::Color(23, 24, 25));
			allCircles.back().setOutlineColor(sf::Color(0, 90, 170));
			allCircles.back().setPosition(cos(360.0f / (float)Vector.size() * c * DegToRad) * 200 + win.getSize().x / 2, -sin(360.0f / (float)Vector.size() * c * DegToRad) * 200 + win.getSize().y / 2);
		}

		/*Vertices*/
		allVertices.reserve(5 + Vector.size());
		for (int c = 0; c < Vector.size(); c++) {
			for (int cc = 0; cc < Vector[c].neighbors.size(); cc++) {
				int neigborSerial = (&*Vector[c].neighbors[cc] - &Vector[0]);
				sf::Vector2f ini = allCircles[c].getPosition(), fin = allCircles[neigborSerial].getPosition();

				allVertices.emplace_back(sf::Vector2f(sqrtf((ini.x - fin.x) * (ini.x - fin.x) + (ini.y - fin.y) * (ini.y - fin.y)), 3));
				allVertices.back().setOrigin(0, 1.5);
				allVertices.back().setFillColor(sf::Color(255, c * 30, c * c * 30, 170));

				allVertices.back().setPosition(ini);
				// size
				allVertices.back().setRotation(atan2f((fin.y - ini.y) * DegToRad, (fin.x - ini.x) * DegToRad) * RadToDeg);
			}
		}


		for (int c = 0; c < allVertices.size(); c++) { win.draw(allVertices[c]); }
		for (int c = 0; c < allCircles.size(); c++) { win.draw(allCircles[c]); }

		win.display();
	}

public:

	Graph() {
		Vector.reserve(5);
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		win.create(sf::VideoMode(500, 500), "Graph", sf::Style::Close, settings);
		win.setVisible(0);
	}
	void newItem(int serial) {
		Vector.emplace_back(serial);
		setGraph();
	}
	void link(int Index, int Link) {
		if (Vector.size() > Index && Vector.size() > Link)
			Vector.at(Index).neighbors.emplace_back(&Vector.at(Link));
	}

	void printCorner(const Corner& corner) {
		std::cout << "\n" << corner.item << ":\t";
		for (int c = 0; c < corner.neighbors.size(); c++) {
			std::cout << " " << corner.neighbors[c]->item;
		}
	}
	void printGraph() {
		std::cout << "\n";
		for (int c = 0; c < Vector.size(); c++) {
			printCorner(Vector[c]);
		}
		std::cout << "\n";
	}

	void updateWin() {
		if (win.isOpen()) {
			sf::Event evnt;
			while (win.pollEvent(evnt)) { if (evnt.type == evnt.Closed || evnt.key.code == sf::Keyboard::Escape) { win.close(); break; } }

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				/*Finding Closest*/
				for (int c = 0; c < allCircles.size(); c++) {
					sf::Vector2f tempPos = allCircles[c].getPosition();

					if (40 * 40 >=
						(tempPos.x - (float)sf::Mouse::getPosition(win).x) * (tempPos.x - (float)sf::Mouse::getPosition(win).x) +
						(tempPos.y - (float)sf::Mouse::getPosition(win).y) * (tempPos.y - (float)sf::Mouse::getPosition(win).y)) {

						allCircles[c].setPosition(sf::Vector2f(sf::Mouse::getPosition(win)));


						/*Vertices*/
						for (int i = 0, v = 0; i < Vector.size(); i++) {
							for (int j = 0; j < Vector[i].neighbors.size(); j++) {

								int neigborSerial = (&*Vector[i].neighbors[j] - &Vector[0]);

								sf::Vector2f ini = allCircles[i].getPosition(), fin = allCircles[neigborSerial].getPosition();

								allVertices[v].setSize(sf::Vector2f(sqrtf((ini.x - fin.x) * (ini.x - fin.x) + (ini.y - fin.y) * (ini.y - fin.y)), 3));
								allVertices[v].setPosition(ini);
								allVertices[v].setRotation(atan2f((fin.y - ini.y) * DegToRad, (fin.x - ini.x) * DegToRad) * RadToDeg);
								v++;
							}
						}

						/*Drawing*/
						win.clear(sf::Color(0, 0, 0));
						for (int c = 0; c < allVertices.size(); c++) { win.draw(allVertices[c]); }
						for (int c = 0; c < allCircles.size(); c++) { win.draw(allCircles[c]); }

						break;
					}
				}
			}

			win.display();
		}
	};

	~Graph() { ; }

};