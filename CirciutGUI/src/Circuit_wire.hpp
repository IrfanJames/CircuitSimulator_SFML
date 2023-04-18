#pragma once

#include "SFML/Graphics.hpp"

class Wire {
private:
	static const int m_width = 2;
	bool status_stopped = false;
	std::vector<sf::RectangleShape> wire;
	std::vector<sf::Vector2f> edge_points;

public:

	Wire(const sf::Vector2f& IniPoint);

	~Wire();

	void makeWire(sf::Vector2f& Pos);

	void makeWire();

	void newEdge();

	void stop();

public:
	bool stopped() const;

	sf::Vector2f initial() const;

	sf::Vector2f end() const;

	void draw(sf::RenderWindow& App) const;
};