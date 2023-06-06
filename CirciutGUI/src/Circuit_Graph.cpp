
#include <iostream>

#include "Circuit_Global.hpp"
#include "LOG.hpp"

#include "Circuit_Graph.hpp"


Graph::Graph()
{
	Vector.reserve(5);
}

Graph::~Graph() { ; }



void Graph::newItem(int serial) {
	Vector.emplace_back(serial);
	setGraph();
}

void Graph::link(int corner1, int corner2) {
	if (Vector.size() > corner1 && Vector.size() > corner2)
		Vector.at(corner1).neighbors.emplace_back(corner2);
	setGraph();
}

void Graph::clearAll() {
	Vector.clear();
	allCircles.clear();
	allVertices.clear();
}



void Graph::createWindow()
{
	draggingCircle = -1;

	if (win.isOpen() == false)
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		win.create(sf::VideoMode(500, 500), "Graph", sf::Style::Default, settings);
	}
	else {
		mouseHold = sf::Vector2f(0, 0);
		viewCenter = sf::Vector2f(
			(int)(win.getSize().x / 2),
			(int)(win.getSize().y / 2));

		win.setView(sf::View(viewCenter, (sf::Vector2f)win.getSize()));
	}

}

void Graph::updateWin() {

	if (win.isOpen()) {

		while (win.pollEvent(evnt))
		{
			if (evnt.type == evnt.Closed)
			{
				win.close();

				return;
			}

			if (evnt.type == evnt.Resized)
			{
				win.setView(sf::View(sf::FloatRect(
					(int)win.getView().getCenter().x - (int)(evnt.size.width / 2),
					(int)win.getView().getCenter().y - (int)(evnt.size.height / 2),
					(int)evnt.size.width,
					(int)evnt.size.height)));
			}

			if (evnt.type == evnt.MouseButtonPressed) {
				if (evnt.mouseButton.button == sf::Mouse::Middle)
				{
					mouseHold = (sf::Vector2f)sf::Mouse::getPosition(win);

					viewCenter = win.getView().getCenter();
				}
			}

			if (evnt.type == evnt.MouseButtonReleased) {
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					draggingCircle = -1;
				}
			}

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == sf::Keyboard::Escape) {
					win.close();
					return;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
			if (win.hasFocus())
			{
				sf::Vector2f newViewCenter(
					viewCenter.x + mouseHold.x - (float)sf::Mouse::getPosition(win).x,
					viewCenter.y + mouseHold.y - (float)sf::Mouse::getPosition(win).y);

				sf::View newView(newViewCenter, (sf::Vector2f)win.getSize());

				win.setView(newView);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			// Finding Closest
			{
				sf::Vector2f cursorWorldPosition = win.mapPixelToCoords(sf::Mouse::getPosition(win));

				if (draggingCircle >= 0 && draggingCircle <= allCircles.size() - !allCircles.empty())
				{
					allCircles[draggingCircle].setPosition(sf::Vector2f(cursorWorldPosition));
				}
				else
				{
					for (int c = 0; c < allCircles.size(); c++) {
						sf::Vector2f tempPos = allCircles[c].getPosition();

						if (40 * 40 >=
							(tempPos.x - (float)cursorWorldPosition.x) * (tempPos.x - (float)cursorWorldPosition.x) +
							(tempPos.y - (float)cursorWorldPosition.y) * (tempPos.y - (float)cursorWorldPosition.y)) {

							allCircles[c].setPosition(sf::Vector2f(cursorWorldPosition));
							draggingCircle = c;

							// Vertices
							//for( ...
							// 
							// Drawing
							//win.clear();
							//for (int c = 0; c < allVertices.size(); c++) { win.draw(allVertices[c]); }
							//for (int c = 0; c < allCircles.size(); c++) { win.draw(allCircles[c]); }
							//win.display();
							break;
						}

						if (c < allCircles.size() - !allCircles.empty())
							draggingCircle = -1;
					}
				}
			}


			// Vertices
			{
				if(draggingCircle != -1)
				{
					for (int i = 0, v = 0; i < Vector.size(); i++) {
						for (int j = 0; j < Vector[i].neighbors.size(); j++)
						{

							int neigborSerial = (&Vector[Vector[i].neighbors[j]] - &Vector[0]);

							sf::Vector2f ini = allCircles[i].getPosition(), fin = allCircles[neigborSerial].getPosition();

							allVertices[v].setSize(sf::Vector2f(sqrtf((ini.x - fin.x) * (ini.x - fin.x) + (ini.y - fin.y) * (ini.y - fin.y)), 3));
							allVertices[v].setPosition(ini);
							allVertices[v].setRotation(atan2f((fin.y - ini.y) * DegToRad, (fin.x - ini.x) * DegToRad) * RadToDeg);
							v++;
						}
					}
				}
			}

		}

		// Drawing
		{
			win.clear();

			for (auto& v : allVertices)
				win.draw(v);
			for (auto& circle : allCircles)
				win.draw(circle);

			win.display();
		}
	}

}



void Graph::printCorner(const Corner& corner) {
	std::cout << "\n" << corner.item << ":\t";
	for (int c = 0; c < corner.neighbors.size(); c++) {
		std::cout << " " << Vector[corner.neighbors[c]].item;
	}
}

void Graph::printGraph() {
	std::cout << "\n";
	for (int c = 0; c < Vector.size(); c++) {
		printCorner(Vector[c]);
	}
	std::cout << "\n";
}



void Graph::setGraph() {

	sf::Vector2f winSize;

	winSize = (win.isOpen()) ? (sf::Vector2f)win.getSize() : sf::Vector2f(500, 500);

	//Edges
	{
		allCircles.reserve(Vector.size());

		if (allCircles.size() > Vector.size())
		{
			allCircles.erase(allCircles.begin() + Vector.size(), allCircles.end());
		}

		while (allCircles.size() < Vector.size()) {
			static int radius = 20;
			allCircles.emplace_back(radius, radius);
			allCircles.back().setOrigin(radius, radius);
			allCircles.back().setOutlineThickness(2);
			allCircles.back().setFillColor(sf::Color(23, 24, 25));
			allCircles.back().setOutlineColor(sf::Color(0, 90, 170));
		}

		float radius = std::min(winSize.x, winSize.y) * 0.45f;
		float dAngle = 360.0f / (float)Vector.size() * DegToRad;
		for (int i = 0; i < allCircles.size(); i++)
		{
			allCircles[i].setPosition(
				+cos(dAngle * i) * radius + winSize.x / 2,
				-sin(dAngle * i) * radius + winSize.y / 2);
		}
	}


	//Vertices
	{
		allVertices.clear();
		allVertices.reserve(5 + Vector.size());

		/*if (allVertices.size() > Vector.size())
			allVertices.erase(allVertices.begin() + Vector.size(), allVertices.end());*/

		for (int i = 0, count = 0; i < Vector.size(); i++) {
			for (int j = 0; j < Vector[i].neighbors.size(); j++, count++) {

				//int neigborSerial = (&((&Vector[0])[Vector[c].neighbors[cc]]) - &Vector[0]);
				int neigborSerial = (&Vector[Vector[i].neighbors[j]] - &Vector[0]);

				static sf::Vector2f ini, fin;
				ini = allCircles[i].getPosition();
				fin = allCircles[neigborSerial].getPosition();

				sf::Vector2f size(1, 3);
				size.x = sqrtf(((ini.x - fin.x) * (ini.x - fin.x)) + ((ini.y - fin.y) * (ini.y - fin.y)));
				if ((int)(allVertices.size() - 1) >= count) // Instead of (allVertices.size() - 1 >= count)  // Because at allVectices.size == 0 the '-1' causes Overflow, so Greater the the 'count'
					allVertices[count].setSize(size);
				else
					allVertices.emplace_back(size);

				allVertices.back().setOrigin(0, 1.5);
				allVertices.back().setFillColor(sf::Color(255, i * 30, i * i * 30, 170));

				allVertices.back().setPosition(ini);
				allVertices.back().setRotation(atan2f((fin.y - ini.y) * DegToRad, (fin.x - ini.x) * DegToRad) * RadToDeg);
			}
		}
	}


	//abcwin.setVisible(1);
	//abcwin.clear(sf::Color(0, 0, 0));
	//abcfor (int c = 0; c < allVertices.size(); c++) { win.draw(allVertices[c]); }
	//abcfor (int c = 0; c < allCircles.size(); c++) { win.draw(allCircles[c]); }
	//abcwin.display();

}
