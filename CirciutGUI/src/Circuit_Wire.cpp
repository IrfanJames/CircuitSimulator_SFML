#pragma once

#include <iostream>

#include "LOG.hpp"
#include "Circuit_Wire.hpp"
#include "Circuit_GUI.hpp"

Wire::Wire(const sf::Vector2f& IniPoint)
{
	edge_points.emplace_back(CircuitGUI::trim(IniPoint));

	wire.reserve(5);
	wire.emplace_back();
	wire.back().setFillColor(CircuitGUI::normalCompColor);
}

Wire::~Wire() { ; }


void Wire::makeWire(sf::Vector2f& Pos)
{
	sf::Vector2f next_point = CircuitGUI::trim(Pos);

	if (edge_points.empty() == false)
	{
		sf::Vector2f this_point = edge_points.back();

		if (wire.empty() == false)
		{
			int dx = CircuitGUI::trim((int)((int)next_point.x - (int)this_point.x));
			int dy = CircuitGUI::trim((int)((int)next_point.y - (int)this_point.y));


			if (std::max(std::abs(dx), std::abs(dy)) >= CircuitGUI::gap)
			{
				if (std::abs(dx) < std::abs(dy)) {
					wire.back().setPosition(this_point.x - 1, this_point.y);
					wire.back().setSize(sf::Vector2f(m_width, dy /*+ gap * (dy > gap)*/));
				}
				else {
					wire.back().setPosition(this_point.x, this_point.y - 1);
					wire.back().setSize(sf::Vector2f(dx /*+ gap * (dx > gap)*/, m_width));
				}
			}
		}
		else {
			wire.emplace_back();
			wire.back().setFillColor(CircuitGUI::normalCompColor);
			LOG("\n[Error]: wire.makeWire(), But vec:wire is empty");
		}
	}
	else {
		edge_points.emplace_back(next_point);
		LOG("\n[Error]: wire.makeWire(), But vec:edge_points is empty");
	}
}

void Wire::makeWire()
{
	sf::Vector2f vec = CircuitGUI::cursorInSim();
	makeWire(vec);
}

void Wire::newEdge()
{
	if (1 /*stop == false*/)
	{
		if (wire.empty() == false)
		{
			if (edge_points.empty() == false)
			{
				sf::Vector2f vec(edge_points.back());

				//if (std::abs(wire.back().getSize().x - m_width) < 2.1)
				if (wire.back().getSize().x == m_width)
				{
					//LOG("\nVertical ");
					//Vertical
					vec.y += wire.back().getSize().y;
				}
				else
				{
					//Horizontal
					//LOG("\nHorizontal ");
					vec.x += wire.back().getSize().x;
				}

				edge_points.emplace_back(CircuitGUI::trim(vec));
				//LOG("vec: ");
				//for (auto& i : edge_points) {
				//	LOG("(" << i.x << ", " << i.y << "), ");
				//}
			}
			else {
				LOG("\n[Error]: wire.newEdge(), But edge_points:wire is empty");
			}
		}
		else
		{
			// No making new wire [wire.emplace_back()] Because it will be done below Anyways
			LOG("\n[Error]: wire.newEdge(), But vec:wire is empty");
		}


		wire.emplace_back();
		wire.back().setFillColor(CircuitGUI::normalCompColor);
	}
}

/*void Wire::stop() {
		stop = true;
}*/


/*bool stopped() {
		return stop;
}*/

sf::Vector2f Wire::initial() const
{
	if (edge_points.empty() == false)
		return CircuitGUI::trim(edge_points.front());
	else
	{
		LOG("\n[Error]: wire.initial(), But vec:edge_points is empty");
		return CircuitGUI::zero;
	}
}

sf::Vector2f Wire::end() const
{
	if (edge_points.empty() == false)
		return CircuitGUI::trim(edge_points.back());
	else
	{
		LOG("\n[Error]: wire.end(), But vec:edge_points is empty");
		return CircuitGUI::zero;
	}
}

void Wire::draw(sf::RenderWindow& App) const
{
	for (int c = 0; c < wire.size(); c++)
		App.draw(wire[c]);
}

