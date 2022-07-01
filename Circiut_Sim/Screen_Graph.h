#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Graph {
	float DegToRad = 0.01745329f;
	float RadToDeg = 57.295779f;
	struct Corner {
		int data = 0;
		std::vector<Corner*> neighbors;

		Corner(int Data) {
			data = Data;
			neighbors.reserve(2);
		}
	};
	std::vector<sf::CircleShape> allCircles;
	std::vector<sf::RectangleShape> allVertices;


public:
	std::vector<Corner> Vector;
	sf::RenderWindow* win = new sf::RenderWindow(sf::VideoMode(550, 550), "Graph", sf::Style::Default, sf::ContextSettings(0));
	// View

	Graph() {
		Vector.reserve(2);
	}

	void printCorner(Corner corner) {
		std::cout << "\n" << corner.data << ":\t";
		for (int c = 0; c < corner.neighbors.size(); c++) {
			std::cout << " " << corner.neighbors[c]->data;
		}
	}
	void printGraph() {
		std::cout << "\n";
		for (int c = 0; c < Vector.size(); c++) {
			printCorner(Vector[c]);
		}
		std::cout << "\n";
	}

	void setGraph() {
		win->clear(sf::Color(0, 0, 0));

		/*Edges*/
		allCircles.reserve(Vector.size());
		for (int c = 0; c < Vector.size(); c++) {
			allCircles.emplace_back(30, 30);
			allCircles.back().setOrigin(30, 30);
			allCircles.back().setOutlineThickness(3);
			allCircles.back().setFillColor(sf::Color(23, 24, 25));
			allCircles.back().setOutlineColor(sf::Color(0, 90, 170));

			allCircles.back().setPosition(cos(360 / Vector.size() * c * DegToRad) * 250 + win->getSize().x / 2, -sin(360 / Vector.size() * c * DegToRad) * 250 + win->getSize().y / 2);
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


		for (int c = 0; c < allVertices.size(); c++) { win->draw(allVertices[c]); }
		for (int c = 0; c < allCircles.size(); c++) { win->draw(allCircles[c]); }

		win->display();
	}

	void updateWin() {
		if (win->isOpen()) {
			sf::Event evnt;
			while (win->pollEvent(evnt)) {
				if (evnt.type == evnt.Closed) {
					win->close();
					break;
				}
				if (evnt.type == evnt.KeyPressed) {
					if (evnt.key.code == sf::Keyboard::Escape) { win->close(); break; }
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				/*Finding Closest*/
				for (int c = 0; c < allCircles.size(); c++) {
					sf::Vector2f tempPos = allCircles[c].getPosition();

					if (30 * 30 >= (tempPos.x - (float)sf::Mouse::getPosition(*win).x) * (tempPos.x - (float)sf::Mouse::getPosition(*win).x) +
						(tempPos.y - (float)sf::Mouse::getPosition(*win).y) * (tempPos.y - (float)sf::Mouse::getPosition(*win).y)) {

						allCircles[c].setPosition(sf::Vector2f(sf::Mouse::getPosition(*win)));


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
						win->clear(sf::Color(0, 0, 0));
						for (int c = 0; c < allVertices.size(); c++) { win->draw(allVertices[c]); }
						for (int c = 0; c < allCircles.size(); c++) { win->draw(allCircles[c]); }
						win->display();

						break;
					}
				}
			}
		}
	};

	~Graph() {
		;
	}


};