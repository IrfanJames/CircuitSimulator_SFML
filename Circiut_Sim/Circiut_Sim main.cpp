#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

#include <iostream>
#include <vector>
#include "Screen_Classes.h"

using namespace sf;
using std::cout; using std::to_string; using std::vector;


int main() {

	RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Default, ContextSettings(0));
	sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), sf::Vector2f(app.getSize().x, app.getSize().y));
	app.setVerticalSyncEnabled(1);
	app.setFramerateLimit(60);
	srand(time(NULL));

	time_t frame = clock();
	bool End = 0, Pause = 0, printScreen = 0;
	bool debugBool = 0;

	bool releaseBool = 1;
	bool OptDrag = 0;

	///////////////////////////////////////////////

	Texture t;
	t.loadFromFile("Images/Rayn.png");
	Sprite Rayn(t), Rayn2(t);
	Rayn.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
	Rayn2.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
	Rayn.setPosition(app.getSize().x / 2, app.getSize().y / 2);
	Rayn2.setPosition(app.getSize().x + 10, app.getSize().y + 10);

	int gap = 15, virtualBoarder = 0;
	vector<sf::RectangleShape> vLines;
	vLines.reserve((W + 2 * virtualBoarder) / gap + 2);

	sf::Vector2f tempVect(2, H + 2 * virtualBoarder);
	for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
		vLines.emplace_back(tempVect);
	}

	sf::Color tempColor(100, 105, 110, 20);
	for (int c = 0; c < vLines.size(); c++) {
		vLines[c].setPosition(-virtualBoarder + c * gap, -virtualBoarder);
		tempColor.a = 20 + (c % 5 == 0) * 15;
		vLines[c].setFillColor(tempColor);
	}

	vector<sf::RectangleShape> hLines;
	hLines.reserve((H + 2 * virtualBoarder) / gap + 2);

	tempVect.x = W + 2 * virtualBoarder; tempVect.y = 2;
	for (int c = 0; c <= (H + 2 * virtualBoarder) / gap; c++) {
		hLines.emplace_back(tempVect);
	}
	for (int c = 0; c < hLines.size(); c++) {
		hLines[c].setPosition(-virtualBoarder, -virtualBoarder + c * gap);
		tempColor.a = 20 + (c % 5 == 0) * 15;
		hLines[c].setFillColor(tempColor);
	}

	///////////////////////////////////////////////
	while (!End) {

		float mouseX = Mouse::getPosition(app).x, mouseY = Mouse::getPosition(app).y;
		float viewX = view.getCenter().x, viewY = view.getCenter().y;
		float verX = vLines[0].getPosition().x, verY = vLines[0].getPosition().y;
		float horX = hLines[0].getPosition().x, horY = hLines[0].getPosition().y;


		int verBrightCount = 5, horBrightCount = 5;

		while (app.isOpen() && !End) {

			Event evnt;
			while (app.pollEvent(evnt)) {
				if (evnt.type == evnt.Closed) {
					app.close();
					End = 1;
				}
				if (evnt.type == evnt.Resized) {
					view.setSize(H * ((float)app.getSize().x / (float)app.getSize().y), H);
				}
				if (evnt.type == evnt.MouseButtonReleased) { releaseBool = 1; }
				if (evnt.type == evnt.KeyPressed) {
					if (evnt.key.code == Keyboard::Escape) { app.close(); End = 1; cout << "\n------------------ESC Pressed-----------------\n"; goto END; }
					if (evnt.key.code == Keyboard::Space) { Pause = !Pause; cout << "\n------------------   Pause   -----------------\n"; }
					if (evnt.key.code == Keyboard::R) { cout << "\n------------------   RESET   -----------------\n"; goto END; }
					if (evnt.key.code == Keyboard::P) { printScreen = 1; }
					if (evnt.key.code == Keyboard::N) { debugBool = !debugBool; cout << "\ndebug\n"; }
				}
			}

			///////////////////////////////////////////////

			// ----------------------------------------	Options

			if (Mouse::isButtonPressed(Mouse::Left)) {
				OptDrag = 1;
				if (releaseBool) {
					mouseX = (float)Mouse::getPosition(app).x; mouseY = (float)Mouse::getPosition(app).y;
					viewX = view.getCenter().x, viewY = view.getCenter().y;
					verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
					horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
				}
				releaseBool = 0;
			}
			else {
				OptDrag = 0;
			}

			// ----------------------------------------	Update

			if (OptDrag) {
				view.setCenter(sf::Vector2f(viewX + mouseX - (float)Mouse::getPosition(app).x, viewY + mouseY - (float)Mouse::getPosition(app).y));
				float newVerY = verY + mouseY - (float)Mouse::getPosition(app).y;
				float newHorX = horX + mouseX - (float)Mouse::getPosition(app).x;

				float verBrightX = vLines[verBrightCount].getPosition().x;
				for (int c = 0; c < vLines.size(); c++) {
					vLines[c].setPosition(newHorX - (int)newHorX % gap + c * gap, newVerY);
				}

				float horBrightY = hLines[horBrightCount].getPosition().y;
				for (int c = 0; c < hLines.size(); c++) {
					hLines[c].setPosition(newHorX, newVerY - (int)newVerY % gap + c * gap);
				}

				verBrightX -= vLines[verBrightCount].getPosition().x;
				if (verBrightX > gap / 2) {
					verBrightCount--;
					verBrightCount = (verBrightCount < 1) * (5 + verBrightCount % 5) + (1 <= verBrightCount) * ((verBrightCount - 1) % 5 + 1);
				}
				else if (verBrightX < -gap / 2) {
					verBrightCount++;
					verBrightCount = (verBrightCount < 1) * (5 + verBrightCount % 5) + (1 <= verBrightCount) * ((verBrightCount - 1) % 5 + 1);
				}
				for (int c = 0; c < vLines.size(); c++) {
					tempColor.a = 20 + ((c + verBrightCount) % 5 == 0) * 15;
					vLines[c].setFillColor(tempColor);
				}

				horBrightY -= hLines[horBrightCount].getPosition().y;
				if (horBrightY > gap / 2) {
					horBrightCount--;
					horBrightCount = (horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1);
				}
				else if (horBrightY < -gap / 2) {
					horBrightCount++;
					horBrightCount = (horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1);
				}
				for (int c = 0; c < hLines.size(); c++) {
					tempColor.a = 20 + ((c + horBrightCount) % 5 == 0) * 15;
					hLines[c].setFillColor(tempColor);
				}

			}



			// ----------------------------------------	Draw
			if (!Pause) {
				app.clear(sf::Color(23, 24, 25));
				for (int c = 0; c < vLines.size(); c++) {
					app.draw(vLines[c]);
				}
				for (int c = 0; c < hLines.size(); c++) {
					app.draw(hLines[c]);
				}
				app.setView(view);


				app.draw(Rayn);
				app.draw(Rayn2);




				app.display();
			}

			app.setTitle("CircuitSIm   " + to_string((float)(CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
			frame = clock();


			printScreen = 0;
		}

	END:
		;
		Pause = 0; printScreen = 0;
	}

	system("pause");
}
