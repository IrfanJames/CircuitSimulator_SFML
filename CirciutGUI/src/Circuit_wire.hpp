#pragma once

#include "Circuit_Entity.hpp"

#include "SFML/Graphics.hpp"

class Wire : public Entity
{

public:
	
	Wire(const sf::Vector2f& IniPoint);

	Wire(const std::string& Serialized_wire);

	~Wire();

	void makeWire(const sf::Vector2f& Pos);

	void makeWire();

	void newEdge();

	void clean();

	void stop();

	void move(const sf::Vector2f& Pos);

	void deSerialize(const std::string& Serialized_wire);

public:

	bool isStopped() const;

	sf::Vector2f initial() const;

	sf::Vector2f end() const;

	size_t noOfEdges() const;

	size_t noOfDrawRects() const;

	const std::vector<sf::RectangleShape>* getRectVector() const;

	void draw(sf::RenderWindow& App) const;


	sf::FloatRect bounds(bool* sucess_ptr = nullptr) const;

	bool contains(const sf::Vector2f& Point) const;

	bool intersectes(const sf::FloatRect& Area) const;

	std::string serialize() const;

private:
	static const int m_width = 2;
	bool status_stopped = false;
	sf::Vector2f DragOffset;
	std::vector<sf::RectangleShape> wire;
	std::vector<sf::Vector2f> edge_points;
};