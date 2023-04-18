#pragma once

#include "SFML/Graphics.hpp"

class Wire {
private:
	static const int m_width = 2;
	//bool stop = false;
	std::vector<sf::RectangleShape> wire;
	std::vector<sf::Vector2f> edge_points;

public:

	Wire(const sf::Vector2f& IniPoint);

	~Wire();

private:

	void makeWire(sf::Vector2f& Pos);
public:

	void makeWire();

	void newEdge();

public:

	sf::Vector2f initial() const;

	sf::Vector2f end() const;

	void draw(sf::RenderWindow& App) const;
};