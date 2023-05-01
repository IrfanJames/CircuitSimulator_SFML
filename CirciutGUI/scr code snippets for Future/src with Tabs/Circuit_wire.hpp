#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

#include "Circuit_GUI.hpp"

class Wire {
private:
	std::vector<sf::RectangleShape> wire;

public:

	Wire(const sf::Vector2f& StartPoint) {
		wire.reserve(2);

		wire.emplace_back(sf::Vector2f(3, 20));
		wire.back().setOrigin(1, 0);
		wire.back().setPosition(StartPoint);
		wire.back().setFillColor(CircuitGUI::normalCompColor);
	}

	void newEdge() {
		sf::Vector2f nextPos(wire.back().getPosition());

		if (wire.size() % 2 == 1)
			nextPos.y += wire.back().getSize().y;
		else if (wire.size() % 2 == 0)
			nextPos.x += wire.back().getSize().x;

		nextPos.x = CircuitGUI::trim(nextPos.x, CircuitGUI::gap);
		nextPos.y = CircuitGUI::trim(nextPos.y, CircuitGUI::gap);

		wire.emplace_back(sf::Vector2f(wire.back().getSize().y, wire.back().getSize().x));
		wire.back().setOrigin(wire.back().getOrigin().y, wire.back().getOrigin().x);
		wire.back().setPosition(nextPos);
		wire.back().setFillColor(CircuitGUI::normalCompColor);
		//wire.back().setRotation((90 * wire.size()) % 360);
	}

	void makeWire() {
		if (wire.size() % 2 == 1)
			wire.back().setSize(sf::Vector2f(
								wire.back().getSize().x,
								0 + CircuitGUI::trim((int)CircuitGUI::cursorInSim().y - wire.back().getPosition().y, CircuitGUI::gap)
								));

		else if (wire.size() % 2 == 0)
			wire.back().setSize(sf::Vector2f(
								0 + CircuitGUI::trim((int)CircuitGUI::cursorInSim().x - wire.back().getPosition().x, CircuitGUI::gap),
								wire.back().getSize().y
								));
	}

	void draw(sf::RenderWindow& App) const {
		for (int c = 0; c < wire.size(); c++)
			App.draw(wire[c]);
	}

public:

	sf::Vector2f getFrontPos() const {
		return wire.front().getPosition();
	}

	sf::Vector2f getEndPos() const {
		sf::Vector2f temp = wire.back().getPosition() + wire.back().getSize();

		temp.x = CircuitGUI::trim(temp.x, CircuitGUI::gap);
		temp.y = CircuitGUI::trim(temp.y, CircuitGUI::gap);

		//std::cout << "\n" << temp.x << ", " << temp.y;

		return temp;
	}

	~Wire() { ; }

};