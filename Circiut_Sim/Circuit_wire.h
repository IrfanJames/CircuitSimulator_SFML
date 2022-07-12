#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

sf::Vector2f cursorInSim();
bool Click(int Sensitivity = 0);
float trim(float num, int wrt);


class Wire {
	sf::Color normalColor;
public:
	std::vector<sf::RectangleShape> wire;

	Wire(sf::Vector2f IniPoint) {
		wire.reserve(2);
		normalColor.r = 255; normalColor.g = 255; normalColor.b = 255;

		wire.emplace_back(sf::Vector2f(3, 20));
		wire.back().setOrigin(1, 0);
		wire.back().setPosition(IniPoint);
		wire.back().setFillColor(normalColor);
	}

	void newEdge() {
		sf::Vector2f nextPos(wire.back().getPosition());
		if (wire.size() % 2 == 1) { nextPos.y += wire.back().getSize().y; }
		else
			if (wire.size() % 2 == 0) { nextPos.x += wire.back().getSize().x; }

		nextPos.x = trim(nextPos.x, 15);
		nextPos.y = trim(nextPos.y, 15);

		wire.emplace_back(sf::Vector2f(wire[wire.size() - 1].getSize().y, wire[wire.size() - 1].getSize().x));
		wire.back().setOrigin(wire[wire.size() - 1].getOrigin().y, wire[wire.size() - 1].getOrigin().x);
		wire.back().setPosition(nextPos);
		wire.back().setFillColor(normalColor);
		//wire.back().setRotation((90 * wire.size()) % 360);
	}

	void makeWire(sf::RenderWindow& App) {
		if (wire.size() % 2 == 1)      wire.back().setSize(sf::Vector2f(wire.back().getSize().x, trim(cursorInSim().y - wire.back().getPosition().y, 15) + 3));
		else if (wire.size() % 2 == 0) wire.back().setSize(sf::Vector2f(trim(cursorInSim().x - wire.back().getPosition().x, 15) + 3, wire.back().getSize().y));
	}

	void draw(sf::RenderWindow& App) {
		for (int c = 0; c < wire.size(); c++) {
			App.draw(wire[c]);
		}
	}

	~Wire() { ; }

};