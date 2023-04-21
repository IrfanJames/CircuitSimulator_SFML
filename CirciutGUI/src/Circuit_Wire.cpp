#pragma once

#include <iostream>
#include <string>
#include <string.h>

#include "LOG.hpp"
#include "Circuit_Wire.hpp"
#include "Circuit_GUI.hpp"

Wire::Wire(const sf::Vector2f& IniPoint)
{
	edge_points.emplace_back(CircuitGUI::trim(IniPoint));

	//wire.reserve(5);
	//wire.emplace_back();
	//wire.back().setFillColor(CircuitGUI::normalCompColor);
}

Wire::Wire(const std::string& serialized_wire)
{
	deSerialize(serialized_wire);
}

Wire::~Wire() { ; }


void Wire::makeWire(const sf::Vector2f& Pos)
{
	sf::Vector2f next_point = CircuitGUI::trim(Pos);

	if (edge_points.empty() == false)
	{
		sf::Vector2f this_point = edge_points.back();

		int dx = CircuitGUI::trim((int)((int)next_point.x - (int)this_point.x));
		int dy = CircuitGUI::trim((int)((int)next_point.y - (int)this_point.y));

		if (std::max(std::abs(dx), std::abs(dy)) >= CircuitGUI::gap)
		{
			if (std::abs(dx) < std::abs(dy)) {
				wire.emplace_back(sf::Vector2f(m_width, dy /*+ gap * (dy > gap)*/));
				wire.back().setPosition(this_point.x - 1, this_point.y);
				wire.back().setFillColor(CircuitGUI::normalCompColor);
			}
			else {
				wire.emplace_back(sf::Vector2f(dx /*+ gap * (dx > gap)*/, m_width));
				wire.back().setPosition(this_point.x, this_point.y - 1);
				wire.back().setFillColor(CircuitGUI::normalCompColor);
			}
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
	if (status_stopped == false)
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

void Wire::stop()
{
	status_stopped = true;
}

void Wire::deSerialize(const std::string& str)
{
	LOG("\nDeserializing: " << str);

	static std::vector<int> Integers;
	Integers.clear();
	CircuitGUI::str_to_vecInt(str, Integers);

	for (auto& num : Integers)
		num = (int)CircuitGUI::trim(num);

	LOG("\nTrimmed" << Integers.size() << ": ");
	for (auto& num : Integers)
		LOG(num << ", ");

	bool invalid = false;
	bool ver0_hor1 = false;
	if (Integers.size() >= 4)
	{
		edge_points.reserve((Integers.size() - 2));

		if (Integers[1] == Integers[3]) {
			ver0_hor1 = true;

			edge_points.emplace_back(Integers[0], Integers[1]);
			makeWire(sf::Vector2f(Integers[2], Integers[3]));
			edge_points.emplace_back(Integers[2], Integers[3]);
		}
		else if (Integers[0] == Integers[2])
		{
			ver0_hor1 = false;

			edge_points.emplace_back(Integers[0], Integers[1]);
			makeWire(sf::Vector2f(Integers[2], Integers[3]));
			edge_points.emplace_back(Integers[2], Integers[3]);
		}
		else {
			invalid = true;
			LOG("\nInvaid Wire::deSerialize(const std::string& str)");
		}
	}

	if (invalid == false) {
		LOG("\n");
		for (int i = 4, ep = 1; i < Integers.size(); i++, ep++) {

			if (ver0_hor1)
			{
				makeWire(sf::Vector2f(edge_points[ep].x, Integers[i]));
				edge_points.emplace_back(edge_points[ep].x, Integers[i]);
			}
			else
			{
				makeWire(sf::Vector2f(Integers[i], edge_points[ep].y));
				edge_points.emplace_back(Integers[i], edge_points[ep].y);
			}

			LOG_VEC2(wire.back().getPosition());
			ver0_hor1 = !ver0_hor1;
		}
	}

	LOG("\nWire(" << wire.size() << "): ");
	for (auto& r : wire)
		LOG_VEC2(r.getPosition());

	stop();
}


bool Wire::isStopped() const
{
	return status_stopped;
}

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

std::string Wire::serialize() const
{
	std::string out = "";
	//std::string out = std::to_string(edge_points.size()) + ": ";

	if (edge_points.size() >= 2)
	{
		LOG("\nSerializing: ")
		for (auto& vec : edge_points)
			LOG_VEC2(vec);


		// 1st and 2nd Points
		{
			out += std::to_string((int)edge_points.at(0).x) + "\t" + std::to_string((int)edge_points.at(0).y) + "\t";
			out += std::to_string((int)edge_points.at(1).x) + "\t" + std::to_string((int)edge_points.at(1).y) + "\t";
		}

		bool ver0_hor1 = ( (int)edge_points.at(0).y == (int)edge_points.at(1).y );

		for (int i = 2; i < edge_points.size(); i++)
		{
			if (ver0_hor1)
				out += std::to_string((int)edge_points.at(i).y) + "\t";
			else													   
				out += std::to_string((int)edge_points.at(i).x) + "\t";

			ver0_hor1 = !ver0_hor1;
		}

	}

	if (out.empty() == false && out.back() == '\t') out.pop_back();
	out.push_back('\n');

	return out;
}

void Wire::draw(sf::RenderWindow& App) const
{
	for (int c = 0; c < wire.size(); c++)
		App.draw(wire[c]);
}

