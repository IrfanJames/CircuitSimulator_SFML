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

	Wire(const std::string& Serialized_wire);

	~Wire();

	void makeWire(const sf::Vector2f& Pos);

	void makeWire();

	void newEdge();

	void stop();

	void move(const sf::Vector2f& Pos);

	void deSerialize(const std::string& Serialized_wire);

public:

	bool isStopped() const;

	sf::Vector2f initial() const;

	sf::Vector2f end() const;

	size_t size() const;

	bool contains(const sf::Vector2f& Point) const;

	bool intersectes(const sf::FloatRect& Area) const;

	std::string serialize() const;

	void draw(sf::RenderWindow& App) const;
};