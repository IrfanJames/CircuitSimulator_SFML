#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

sf::Vector2f cursorInSim();
bool Click(int Sensitivity = 0);
float trim(float num, int wrt);

class Wire {
private:
	sf::Color normalColor;
	std::vector<sf::RectangleShape> wire;

public:

	Wire(sf::Vector2f IniPoint) {
		wire.reserve(2);
		normalColor.r = 230; normalColor.g = 230; normalColor.b = 230;

		wire.emplace_back(sf::Vector2f(3, 20));
		wire.back().setOrigin(1, 0);
		wire.back().setPosition(IniPoint);
		wire.back().setFillColor(normalColor);
	}

	void newEdge() {
		sf::Vector2f nextPos(wire.back().getPosition());
		if (wire.size() % 2 == 1) { nextPos.y += wire.back().getSize().y; } else
		if (wire.size() % 2 == 0) { nextPos.x += wire.back().getSize().x; }

		nextPos.x = trim(nextPos.x, 15);
		nextPos.y = trim(nextPos.y, 15);

		wire.emplace_back(sf::Vector2f(wire.back().getSize().y, wire.back().getSize().x));
		wire.back().setOrigin(wire.back().getOrigin().y, wire.back().getOrigin().x);
		wire.back().setPosition(nextPos);
		wire.back().setFillColor(normalColor);
		//wire.back().setRotation((90 * wire.size()) % 360);
	}

	void makeWire(sf::RenderWindow& App) {
		if (wire.size() % 2 == 1)      wire.back().setSize(sf::Vector2f(wire.back().getSize().x, trim((int)cursorInSim().y - wire.back().getPosition().y, 15)));
		else if (wire.size() % 2 == 0) wire.back().setSize(sf::Vector2f(trim((int)cursorInSim().x - wire.back().getPosition().x, 15), wire.back().getSize().y));
	}

	void draw(sf::RenderWindow& App) {
		for (int c = 0; c < wire.size(); c++) App.draw(wire[c]);
	}

public:

	sf::Vector2f initial() {
		return wire.front().getPosition();
	}

	sf::Vector2f end() {
		sf::Vector2f temp = wire.back().getPosition() + wire.back().getSize();

		temp.x = trim(temp.x, 15);
		temp.y = trim(temp.y, 15);

		std::cout << "\n" << temp.x << ", " << temp.y;
		
		return temp;
	}

	~Wire() { ; }

};