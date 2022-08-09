#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

//#include "imgui.h"
//#include "imgui-SFML.h"
#include <iostream>
//#include <thread>
#include <fstream>
//#include <future>
//#include <vector>

#include "Circuit_Entity.h"
//#include "Circuit_Graph.h"
//#include "Circuit_wire.h"

#include "clipboardxx.hpp"

using namespace sf;
using std::cout;

sf::Vector2f cursorInSim();
bool Click(int Sensitivity);
float trim(float num, int wrt);

bool occupiedAt(int Index, sf::Vector2f At);
bool compIn(Entity Comp, sf::Vector2f Ini, sf::Vector2f Fin);
void getBounds(Entity Comp, int arr[4]);

void printScreen();

void updateAllEnds();

//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
sf::RenderWindow app;
float mouseHoldX = Mouse::getPosition(app).x, mouseHoldY = Mouse::getPosition(app).y;

sf::View view(sf::Vector2f(W / 2, H / 2), sf::Vector2f(W, H));
std::vector<Entity> comp;
std::vector<sf::Vector2f> allEnds;

bool Occupied = 0;


int main() {

	sf::ContextSettings settings; settings.antialiasingLevel = 8;
	app.create(VideoMode(W, H), "CircuitSim", Style::Default, settings);
	//app.create(VideoMode(W, H), "CircuitSim", Style::Default, ContextSettings(0));

	W = app.getSize().x; H = app.getSize().y;
	app.setVerticalSyncEnabled(1);
	app.setFramerateLimit(60);
	srand(time(NULL));

	app.setPosition(sf::Vector2i(100, 100));

	//ImGui::SFML::Init(app);//
	//sf::Clock deltaClock;//

	time_t frame = clock();
	bool End = 0, debugBool = 0;

	bool stimuliDisplay = 1, stimuliEndNodes = 1;

	bool releaseBool = 1;

	bool Drag = 0, selectSquare = 0, mouseOnCompsBool = 0;
	bool onceOptMounsePressed = 0, delComp = 0, rotComp = 0;
	bool wireBool = 0;
	bool save = 0, saveNew = 0, open = 0, printScreenBool = 0, copy = 0, paste = 0, onceOptComp = 0;

	int serialCompMouse = 0, serialToolMouse = 0;

	sf::Color normalCompColor(255, 255, 255), tempDimColor(150, 150, 150);

	/*Wires*/
	/*std::vector<Wire> wires;
	wires.reserve(3);
	wires.emplace_back(sf::Vector2f(300, 300));*/

	///////////////////////////////////////////////

	/*Rayn*/ //Texture t; t.loadFromFile("Images/0 Rayn.png"); Sprite Rayn(t), Rayn2(t); Rayn.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn2.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn.setPosition(app.getSize().x / 2, app.getSize().y / 2); Rayn2.setPosition(app.getSize().x + 10, app.getSize().y + 10);

	bool DrawCircle = 1;
	float t_radius = 60, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
	int t_vertices = 34;
	sf::CircleShape testCircle(t_radius, t_vertices);
	testCircle.setOrigin(t_radius, t_radius);
	testCircle.setPosition(W / 2, H / 2);
	testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));

	/*Grid*/
	int gap = 15, virtualBoarder = 80;
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;
	sf::Color gridColor(100, 105, 110, 20);
	{
		vLines.reserve((W + 2 * virtualBoarder) / gap + 2);
		sf::Vector2f tempVect(2, H + 2 * virtualBoarder);
		for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
			vLines.emplace_back(tempVect);
		}
		for (int c = 0; c < vLines.size(); c++) {
			vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);
			gridColor.a = 20 + (c % 5 == 0) * 15;
			vLines[c].setFillColor(gridColor);
		}

		hLines.reserve((H + 2 * virtualBoarder) / gap + 2);
		tempVect.x = W + 2 * virtualBoarder; tempVect.y = 2;
		for (int c = 0; c <= (H + 2 * virtualBoarder) / gap; c++) {
			hLines.emplace_back(tempVect);
		}
		for (int c = 0; c < hLines.size(); c++) {
			hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);
			gridColor.a = 20 + (c % 5 == 0) * 15;
			hLines[c].setFillColor(gridColor);
		}
	}

	/*Tool Textures*/ {
		compTex[0].loadFromFile("Images/Cap.png");
		compTex[1].loadFromFile("Images/Cur.png");
		compTex[2].loadFromFile("Images/Dod.png");
		compTex[3].loadFromFile("Images/Ind.png");
		compTex[4].loadFromFile("Images/Res.png");
		compTex[5].loadFromFile("Images/SwO.png");
		compTex[6].loadFromFile("Images/Vol.png");
		compTex[7].loadFromFile("Images/SwC.png");
	}

	////////////////////////////////////////////// ToolBox
	sf::RectangleShape ToolBoxWin(sf::Vector2f(100, H));
	float t_TollWX = ToolBoxWin.getSize().x;
	float ToolBoxWinRestingPosX = 0, ToolBoxWinRestingPosY = 0;
	ToolBoxWin.setFillColor(sf::Color(0, 0, 0, 120));

	sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(ToolBoxWin.getSize().x, ToolBoxWin.getSize().x));
	float ToolLilWinRestingPosX = 0, ToolLilWinRestingPosY = 0;
	ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));

	sf::Sprite ToolSpr[7]; //noOfComps-1
	sf::Vector2f ToolSprPOS[7]; //noOfComps-1
	for (int c = 0; c < (noOfComps - 1); c++) {
		ToolSpr[c].setTexture(compTex[c]);
		ToolSpr[c].setOrigin(compTex[c].getSize().x / 2, 0);
		//compSpr[c].setRotation(45);
		ToolSprPOS[c].x = ToolBoxWin.getSize().x / 2;
		ToolSprPOS[c].y = c * ToolBoxWin.getSize().x + (int)(ToolBoxWin.getSize().x - compTex[c].getSize().y) / 2;
		ToolSpr[c].setPosition(ToolSprPOS[c].x, ToolSprPOS[c].y);
	}

	////////////////////////////////////////////// VirutalComps
	std::vector<int> virSerial; virSerial.reserve(8);
	std::vector<sf::Sprite> virSprite; virSprite.reserve(8);

	/*Sel Sqr*/
	sf::RectangleShape selSqr; {
		selSqr.setFillColor(sf::Color(0, 0, 0, 0));
		selSqr.setOutlineThickness(1.0f);
		selSqr.setOutlineColor(sf::Color(66, 135, 245)); }


	/*Circuit*/

	comp.reserve(8);
	allEnds.reserve(10);
	//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);


	////////////////////////////////////////////// Solve
	//Graph circuit;

	sf::CircleShape nodePic(4, 15);
	nodePic.setOrigin(4, 4);
	nodePic.setFillColor(normalCompColor);
	std::vector<sf::CircleShape> allEndCircles; allEndCircles.reserve(5);

	//////////////////////////////////////////////

	//std::thread printScreenTread;


	///////////////////////////////////////////////
	while (!End) {

		clipboardxx::clipboard clipboard;

		float viewX = view.getCenter().x, viewY = view.getCenter().y;
		float verX = vLines[0].getPosition().x, verY = vLines[0].getPosition().y;
		float horX = hLines[0].getPosition().x, horY = hLines[0].getPosition().y;

		int verBrightCount = 5, horBrightCount = 5;

		while (app.isOpen() && !End) {

			W = app.getSize().x; H = app.getSize().y;

			float t_TollWx = ToolBoxWin.getPosition().x;
			bool MInTool = !!(0 <= Mouse::getPosition(app).x && Mouse::getPosition(app).x <= t_TollWX);
			bool MIntool = !!(MInTool && Mouse::getPosition(app).y < (noOfComps - 1)* t_TollWX);
			delComp = 0; rotComp = 0; paste = 0;
			Occupied = 0;

			Event evnt;
			while (app.pollEvent(evnt)) {
				stimuliDisplay = 1;
				//ImGui::SFML::ProcessEvent(evnt);

				if (evnt.type == evnt.Closed) { app.close(); End = 1; }
				if (evnt.type == evnt.Resized) {
					view.setCenter(view.getCenter());
					view.setSize(evnt.size.width, evnt.size.height);

					/*Resting Pos of Tool Bar   (Repeated at the at of Drag{}) */ {
						ToolBoxWin.setSize(sf::Vector2f(ToolBoxWin.getSize().x, view.getSize().y));
						ToolBoxWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolBoxWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
						ToolLilWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolLilWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
						for (int c = 0; c < (noOfComps - 1); c++) ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);
					}
				}
				if (evnt.type == evnt.MouseButtonReleased) { releaseBool = 1; }

				if (evnt.type == evnt.KeyPressed) {
					if (evnt.key.code == Keyboard::Escape) { app.close(); End = 1; cout << "\n------------------ESC Pressed-----------------\n"; goto END; }
					//if (evnt.key.code == Keyboard::R) { cout << "\n------------------   Reset   -----------------\n"; goto END; }
					if (evnt.key.code == Keyboard::P) { printScreenBool = 1; }
					if (evnt.key.code == Keyboard::S) { save = 1; }
					if (evnt.key.code == Keyboard::O) { open = 1; }
					if (evnt.key.code == Keyboard::N) { debugBool = !debugBool; /*cout << "\ndebug\n";*/ }
					//if (evnt.key.code == Keyboard::W) { wireBool = !wireBool; /*cout << "\ndebug\n";*/ }
					if (evnt.key.code == Keyboard::Delete) { delComp = 1; stimuliDisplay = 1; }

					/*int difr = 10;
					if (evnt.key.code == Keyboard::Up) { view.setCenter(view.getCenter().x, view.getCenter().y - difr); }
					if (evnt.key.code == Keyboard::Down) { view.setCenter(view.getCenter().x, view.getCenter().y + difr); }
					if (evnt.key.code == Keyboard::Right) { view.setCenter(view.getCenter().x + difr, view.getCenter().y); }
					if (evnt.key.code == Keyboard::Left) { view.setCenter(view.getCenter().x - difr, view.getCenter().y); }*/

				}
			}

			/*else if (evnt.key.code == Keyboard::R) {
				view.setCenter(W / 2, H / 2);
				viewX = view.getCenter().x; viewY = view.getCenter().y;
				for (int c = 0; c < vLines.size(); c++)
					vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);

				for (int c = 0; c < hLines.size(); c++)
					hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);

				verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
				horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
			}*/
			if ((Keyboard::isKeyPressed(Keyboard::R)) && (Keyboard::isKeyPressed(Keyboard::RControl) || Keyboard::isKeyPressed(Keyboard::LControl))) { rotComp = 1; stimuliDisplay = 1; }
			if ((Keyboard::isKeyPressed(Keyboard::S)) && (Keyboard::isKeyPressed(Keyboard::RControl) || Keyboard::isKeyPressed(Keyboard::LControl))) { saveNew = 1; }

			if ((Keyboard::isKeyPressed(Keyboard::C)) && (Keyboard::isKeyPressed(Keyboard::RControl) || Keyboard::isKeyPressed(Keyboard::LControl))) { copy = 1; }
			else {
				copy = 0;
				if ((Keyboard::isKeyPressed(Keyboard::V)) && (Keyboard::isKeyPressed(Keyboard::RControl) || Keyboard::isKeyPressed(Keyboard::LControl))) { paste = 1; stimuliDisplay = 1; }
				else {
					paste = 0;
					onceOptComp = 1;
				}
			}
			///////////////////////////////////////////////







			// ----------------------------------------	Options
			{
				/*Mouse Hold*/
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (releaseBool) {
						releaseBool = 0;
						mouseHoldX = (float)Mouse::getPosition(app).x; mouseHoldY = (float)Mouse::getPosition(app).y;

						//wires[0].newEdge();

						/*new Comp*/
						if (MIntool) {
							MIntool = 0;
							float tempNewCompX = 150 + view.getCenter().x - W / 2, tempNewCompY = 150 + view.getCenter().y - H / 2;
							tempNewCompX = trim(tempNewCompX, gap);
							tempNewCompY = trim(tempNewCompY, gap);

							comp.emplace_back(serialToolMouse, tempNewCompX, tempNewCompY, 0);

							/*Collisions*/
							tempNewCompX = trim(tempNewCompX, gap);
							while (occupiedAt(comp.size() - 1, sf::Vector2f(tempNewCompX, tempNewCompY))) {
								tempNewCompX = trim(tempNewCompX + 6 * gap, gap);

								if (tempNewCompX + 7 * gap - 150 - view.getCenter().x + W / 2 + 91 >= W) {
									tempNewCompX = 150 + view.getCenter().x - W / 2;
									tempNewCompY = trim(tempNewCompY + 6 * gap, gap);
								}
							}

							comp.back().x = tempNewCompX; comp.back().y = tempNewCompY;

							stimuliEndNodes = 1;

						}
						else {
							mouseOnCompsBool = 0;

							/*Check every component for Mouse*/
							for (int c = 0; !mouseOnCompsBool && c < comp.size(); c++) {

								/*Dealing with Origin*/
								int compBoundArr[4];
								getBounds(comp[c], compBoundArr);

								if ((compBoundArr[0] < cursorInSim().x) && (cursorInSim().x < compBoundArr[1])) {
									if ((compBoundArr[2] < cursorInSim().y) && (cursorInSim().y < compBoundArr[3])) {

										mouseOnCompsBool = 1; Drag = 0;

										virSerial.emplace_back(c);

										//copied in sel square selection
										virSprite.emplace_back(comp[virSerial.back()].sprite);
										virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
										virSprite.back().setColor(tempDimColor);/*
										for (int v = 0; v < virSerial.size(); v++) {//copied in sel square selection
											virSprite.emplace_back(comp[virSerial[v]].sprite);
											virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
											virSprite.back().setColor(tempDimColor);
										}*/
									}

								}
							}

							//////////// Urgent need of enums , State Machine
							if (!mouseOnCompsBool && selectSquare) { selSqr.setPosition(cursorInSim()); }

							/*Drag Background*/
							if (!mouseOnCompsBool && !selectSquare) {
								Drag = 1; mouseOnCompsBool = 0;
								viewX = view.getCenter().x, viewY = view.getCenter().y;
								verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
								horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
							}
						}
					}

					if (virSerial.size() > 0) mouseOnCompsBool = 1;

					/*Once while hold*/
					if (mouseOnCompsBool && rotComp) {
						if (onceOptMounsePressed) {
							for (int v = 0; v < virSerial.size(); v++) {
								comp[virSerial[v]].angle += 90;
								comp[virSerial[v]].angle -= (int)(comp[virSerial[v]].angle / 360) * 360;
							}

							stimuliDisplay = 1;
							stimuliEndNodes = 1;
						}
						onceOptMounsePressed = 0;
					}
					else if (mouseOnCompsBool && delComp) {
						if (onceOptMounsePressed) {
							while (0 < virSerial.size()) {
								if (comp.size() > 0) comp.erase(comp.begin() + virSerial[0]);
								if (virSprite.size() > 0) virSprite.erase(virSprite.begin());

								for (int c = 1; c < virSerial.size(); c++) {
									if (virSerial[c] > virSerial[0]) virSerial[c]--;
								}

								virSerial.erase(virSerial.begin());
							}

							stimuliDisplay = 1;
							stimuliEndNodes = 1;
						}
						mouseOnCompsBool = 0;
						onceOptMounsePressed = 0;
					}
					else onceOptMounsePressed = 1;
				}
				else {
					Drag = 0; mouseOnCompsBool = 0;
					/*Click*/

					if (Click(gap)) {
						stimuliDisplay = 1;
						for (int v = 0; v < virSerial.size(); v++) {
							int tempCompClick = comp[virSerial[v]].serial;
							comp[virSerial[v]].serial = (tempCompClick == 5) * 7 + (tempCompClick == 7) * 5 + (tempCompClick != 5 && tempCompClick != 7) * (tempCompClick);
							comp[virSerial[v]].sprite.setTexture(compTex[comp[virSerial[v]].serial]);
						}

					}

					//if (Click(gap)) {
					virSprite.clear();  ///// 
					virSerial.clear();  ///// 
					//}

					/* //ZZzzzz Recolor back to normal    & clear serials
					if (virSerial.size() != 0) {
						for (int v = 0; v < virSerial.size() && comp.size() != 0; v++) {
							//cout << "\nChanging" << virSerial[v];
							comp[virSerial[v]].sprite.setColor(normalCompColor);
						}
					}*/
				}

				/*Continoue while hold*/
				if (mouseHoldX != (float)Mouse::getPosition(app).x || mouseHoldY != (float)Mouse::getPosition(app).y) {

					/*Follow Mouse*/
					if (mouseOnCompsBool) {
						if (!selectSquare /*&& releaseBool*/) {
							int tempRotArr[4][2] = {
								{0, -2},
								{2, 0},
								{0, 2},
								{-2, 0}
							};
							for (int c = 0; c < virSerial.size(); c++) {

								float tempX = cursorInSim().x + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
								float tempY = cursorInSim().y + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///

								//float tempX = cursorInSim().x - mouseHoldX + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
								//float tempY = cursorInSim().y - mouseHoldY + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///


								//for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }
								for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX, tempY); }

								tempX = trim(tempX, gap);
								tempY = trim(tempY, gap);

								if (!occupiedAt(virSerial[c], sf::Vector2f(tempX, tempY))) {
									comp[virSerial[c]].x = tempX; // += (were "=" before)
									comp[virSerial[c]].y = tempY; // += (were "=" before)
								}
							}
						}

						stimuliEndNodes = 1; stimuliDisplay = 1;
					}

				}

				/*Select Sqr*/
				if (selectSquare && !releaseBool) {
					stimuliDisplay = 1;

					/*Sel Sqr*/
					selSqr.setSize(sf::Vector2f(cursorInSim().x - selSqr.getPosition().x, cursorInSim().y - selSqr.getPosition().y));

					/*Selection*/
					for (int c = 0; c < comp.size(); c++) {

						bool compFound = 0;
						int v = 0;
						for (; v < virSerial.size(); v++) {
							if (c == virSerial[v]) { compFound = 1; break; }
						};

						if (compIn(comp[c], selSqr.getPosition(), cursorInSim())) {
							if (!compFound) {
								virSerial.emplace_back(c);

								virSprite.emplace_back(comp[virSerial.back()].sprite);
								/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
								virSprite.back().setColor(tempDimColor);*/
							}
						}
						else {
							if (compFound) {
								virSerial.erase(virSerial.begin() + v);

								virSprite.erase(virSprite.begin() + v);
							}
						}
					}
				}

				//if (wireBool) { Stimuli = 1; wires[0].makeWire(app); }

				if (printScreenBool) {
					printScreenBool = 0;

					time_t print = clock();

					for (int c = 0; c < 1; c++) {
						//std::thread printScreenTread { printScreen }; printScreenTread.join();

						//std::async(std::launch::async, printScreen);

						printScreen();
					}
					cout << "\n" << ((float)clock() - (float)print) / (float)CLOCKS_PER_SEC;
					print = clock();

				}

				if (paste) {
					if (onceOptComp) {
						std::string inString;
						std::vector<int> integers; integers.reserve(9);
						clipboard >> inString;

						//cout << inString;

						bool negative = 0;
						for (int c = 0, x = 0, temp = 0; c < inString.size(); c++) {

							temp = (int)inString[c];

							if (48 <= temp && temp <= 57) {
								x = x * 10 + temp - 48;
							}
							else if (temp == 45) negative = 1;
							else if (temp == 10 || temp == 9) { // else if (temp == (int)('\n') || temp == (int)('\t')) {
								if (negative) x *= -1;
								integers.emplace_back(x);
								negative = 0;
								x = 0;
							}

						}

						//cout << "\n"; for (int c = 0; c < integers.size(); c++) cout << integers[c] << " ";

						for (int c = 0, S = 0, X = 0, Y = 0, A = 0; 1 + c + 4 <= integers.size();) {
							S = integers[++c];
							X = integers[++c];
							Y = integers[++c];
							A = integers[++c];
							comp.emplace_back(S % (noOfComps + 1), trim(X, gap), trim(Y, gap), ((A % 360) / 90) * 90);
						}

						stimuliDisplay = 1;
						stimuliEndNodes = 1;
					}
					onceOptComp = 0; copy = 0;
				}

				if (save || (copy && onceOptComp)) {

					std::ofstream output;

					if (!copy) {
						int fileNo = 0;
						std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
						std::ifstream test(fileDir + std::to_string(fileNo) + fileType);

						while (test.good()) {
							test.close();
							test.open(fileDir + std::to_string(++fileNo) + fileType);
						}

						output.open(fileDir + std::to_string(fileNo) + fileType);

					}

					std::string tempStr;
					if (copy) {
						tempStr = std::to_string((int)virSerial.size()) + "\n";
					}
					else {
						tempStr = std::to_string((int)comp.size()) + "\n";
					}

					int size = ((int)comp.size()) * (!copy) + ((int)virSerial.size()) * (copy);
					for (int c = 0; c < size; c++) {
						if (copy)
							tempStr += std::to_string(comp[virSerial[c]].serial) + "\t" + std::to_string((int)comp[virSerial[c]].x) + "\t" + std::to_string((int)comp[virSerial[c]].y) + "\t" + std::to_string((int)comp[virSerial[c]].angle) + "\n";
						else
							tempStr += std::to_string(comp[c].serial) + "\t" + std::to_string((int)comp[c].x) + "\t" + std::to_string((int)comp[c].y) + "\t" + std::to_string((int)comp[c].angle) + "\n";
					}

					if (copy) {
						clipboard << tempStr;
					}
					else {
						output << tempStr;
						output.close();
					}

					save = 0; saveNew = 0;
					onceOptComp = 0;
					copy = 0;
				}

				if (open) {

					int fileNo = 0;
					std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
					std::ifstream input(fileDir + std::to_string(fileNo) + fileType);

					while (input.good()) {
						input.close();
						input.open(fileDir + std::to_string(++fileNo) + fileType);
					}
					input.open(fileDir + std::to_string(fileNo - 1) + fileType);

					comp.clear();
					virSprite.clear();
					virSerial.clear();
					int no = 0;
					input >> no;
					for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
						input >> S >> X >> Y >> A;
						comp.emplace_back(S % (noOfComps + 1), trim(X, gap), trim(Y, gap), ((A % 360) / 90) * 90);
					}
					input.close();

					stimuliEndNodes = 1;
					open = 0;
				}

			}


			// ----------------------------------------	Update

			//circuit.updateWin();
			if (MInTool) { stimuliDisplay = 1; }
			{
				/*ImGui*/
				/*ImGui::SFML::Update(app, deltaClock.restart());//
						ImGui::Begin("Frist ImGui Win");
						ImGui::Text("My Project will be on Steroids");
						ImGui::Checkbox("Draw Circle", &DrawCircle);
						ImGui::SliderFloat("Radius", &t_radius, 0, 200);
						ImGui::SliderInt("Sides", &t_vertices, 3, 35);
						ImGui::ColorEdit3("Color", t_Colors);
						ImGui::End();



						testCircle.setRadius(t_radius);
						testCircle.setOrigin(testCircle.getRadius(), testCircle.getRadius());
						testCircle.setPointCount(t_vertices);
						testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));*/

				if (Drag) {
					stimuliDisplay = 1;
					view.setCenter(sf::Vector2f(viewX + mouseHoldX - (float)Mouse::getPosition(app).x, viewY + mouseHoldY - (float)Mouse::getPosition(app).y));
					float newVerY = verY + mouseHoldY - (float)Mouse::getPosition(app).y;
					float newHorX = horX + mouseHoldX - (float)Mouse::getPosition(app).x;


					float verBrightX = vLines[verBrightCount].getPosition().x;
					for (int c = 0; c < vLines.size(); c++) {
						vLines[c].setPosition(trim(newHorX, gap) + c * gap, newVerY);
					}
					float horBrightY = hLines[horBrightCount].getPosition().y;
					for (int c = 0; c < hLines.size(); c++) {
						hLines[c].setPosition(newHorX, trim(newVerY, gap) + c * gap);
					}

					verBrightX -= vLines[verBrightCount].getPosition().x;
					if (verBrightX > gap * 0.9) verBrightCount--; else if (verBrightX < -gap * 0.9) verBrightCount++;
					//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + (int)horX % gap) / gap;
					//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + (-virtualBoarder - trim(horX,gap))) / gap;
					//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + horX - (horX / (float)gap)*gap) / gap;
					//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x + (-virtualBoarder + viewX - W / 2) - verX) / gap);
					//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) - (int)horX / gap) / gap;
					//cout << "\n" << mouseX - (float)Mouse::getPosition(app).x << ", " << + (-virtualBoarder + viewX - W / 2) - verX << ", " << verBrightCount;

					verBrightCount = abs((verBrightCount < 1) * (5 + verBrightCount % 5) + (1 <= verBrightCount) * ((verBrightCount - 1) % 5 + 1)) % 6;
					//cout << "\n" << verBrightCount << "\n";
					for (int c = 0; c < vLines.size(); c++) {
						gridColor.a = 20 + ((c + verBrightCount) % 5 == 0) * 15;
						vLines[c].setFillColor(gridColor);
					}


					horBrightY -= hLines[horBrightCount].getPosition().y;
					if (horBrightY > gap * 0.9) horBrightCount--; else if (horBrightY < -gap * 0.9) horBrightCount++;
					horBrightCount = abs((horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1)) % 6;
					//cout << "\n" << horBrightCount << "\n";
					for (int c = 0; c < hLines.size(); c++) {
						gridColor.a = 20 + ((c + horBrightCount) % 5 == 0) * 15;
						hLines[c].setFillColor(gridColor);
					}

					/*Resting Pos of Tool Bar   (Repeated at the at of Resize window) */
					ToolBoxWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolBoxWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					ToolLilWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolLilWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					for (int c = 0; c < (noOfComps - 1); c++) ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);

				}

				//Tool Win
				ToolBoxWin.setPosition((MInTool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (noOfComps - 1)) + (!MInTool) * (t_TollWx + (ToolBoxWinRestingPosX - t_TollWX - t_TollWx) / (noOfComps - 1)), ToolBoxWinRestingPosY);

				//Tool Sqr
				t_TollWx = ToolBoxLittleBox.getPosition().x;
				ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (noOfComps - 1)) + (!MIntool) * (t_TollWx + (ToolBoxWinRestingPosX - t_TollWX - t_TollWx) / (noOfComps - 1)), ToolLilWinRestingPosY + trim(Mouse::getPosition(app).y, (int)ToolBoxLittleBox.getSize().x));
				if (MIntool) serialToolMouse = (int)(Mouse::getPosition(app).y / ToolBoxLittleBox.getSize().x); else serialToolMouse = 0;

				/*endNodes*/ {
					if (stimuliEndNodes) {

						updateAllEnds();

						while (allEnds.size() < allEndCircles.size()) {
							allEndCircles.pop_back();
						}

						while (allEndCircles.size() < allEnds.size()) {
							allEndCircles.emplace_back(nodePic);
						}

						for (int e = 0; e < allEndCircles.size(); e++) {
							allEndCircles[e].setPosition(allEnds[e]);
						}

						//cout << "\n" << allEnds.size();
					}
				}

				testCircle.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 + 200);

			}


			// ----------------------------------------	Draw
			{
				if (1 || stimuliDisplay) { // zero causes 100 cpu load
					app.setView(view);
					app.clear(sf::Color(23, 24, 25));
					//app.draw(Rayn);
					//app.draw(Rayn2);

					/*grid*/ {
						for (int c = 0; c < vLines.size(); c++) { app.draw(vLines[c]); }
						for (int c = 0; c < hLines.size(); c++) { app.draw(hLines[c]); }
					}

					/*comp*/ {
						for (int c = 0; c < comp.size(); c++) { comp[c].draw(app); }
					}

					/*Wires*/ //if (wireBool) { for (int c = 0; c < wires.size(); c++) wires[c].draw(app); }

					/*Nodes*/ {
						for (int e = 0; e < allEndCircles.size(); e++) { app.draw(allEndCircles[e]); }
					}

					if (Occupied) for (int v = 0; v < virSprite.size(); v++) { app.draw(virSprite[v]); }

					if (DrawCircle) app.draw(testCircle);

					selectSquare = debugBool;
					if (selectSquare && !releaseBool) app.draw(selSqr);

					/*Tool Win*/ {
						if (MInTool) {
							app.draw(ToolBoxWin);
							for (int c = 0; c < (noOfComps - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
							for (int c = 0; c < (noOfComps - 1); c++) { app.draw(ToolSpr[c]); }
						}
						else { for (int c = 0; c < (noOfComps - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, 0)); } }

						if (MIntool) app.draw(ToolBoxLittleBox);
					}


					/*ImGui*/
					//ImGui::SFML::Render(app);//Last Thing to render

					app.display();
				}
			}

			app.setTitle("CircuitSIm   " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
			frame = clock();


			stimuliDisplay = 0; stimuliEndNodes = 0;
		}

	END:
		;
		stimuliDisplay = 1; stimuliEndNodes = 1;
	}



	//ImGui::SFML::Shutdown();
	system("pause");
	return 0;
}


void updateAllEnds() {

	allEnds.clear();

	int nodeCount = 0;
	for (int c = 0; c < comp.size(); c++) {
		static sf::Vector2f tempEnd;

		for (int cc = 0; cc < 2; cc++) {

			//tempEnd
			if (cc == 0) { /*Front*/ tempEnd.x = comp[c].x; tempEnd.y = comp[c].y; }
			if (cc == 1) { /*Rear*/  tempEnd = comp[c].endNodePos(); }

			bool found = 0;
			for (int e = 0; e < allEnds.size(); e++) {
				if (tempEnd == allEnds[e]) {
					found = 1;
					break;
				}
			}

			if (!found) {
				nodeCount++;

				if (nodeCount <= allEnds.size()) allEnds[nodeCount] = tempEnd;
				else allEnds.emplace_back(tempEnd);
			}

		}
	}

	while (nodeCount < allEnds.size())
		allEnds.pop_back();

}

void printScreen() {
	sf::Image OutputImage;
	int compBoundArr[4];
	int A = 0, B = 0, C = 0, D = 0;

	if (comp.size() != 0) {
		getBounds(comp[0], compBoundArr);
		A = compBoundArr[0], B = compBoundArr[1], C = compBoundArr[2], D = compBoundArr[3]; // Borders

		for (int c = 0; c < comp.size(); c++) {
			getBounds(comp[c], compBoundArr);
			if (compBoundArr[0] < A) A = compBoundArr[0];
			if (compBoundArr[1] > B) B = compBoundArr[1];
			if (compBoundArr[2] < C) C = compBoundArr[2];
			if (compBoundArr[3] > D) D = compBoundArr[3];
		}
		/*for (int c = 0; c < comp.size(); c++) {
			sf::Vector2f tempEndNode = endNodePos(comp[c]);
			if (tempEndNode.x < A) A = tempEndNode.x;
			if (tempEndNode.x > B) B = tempEndNode.x;
			if (tempEndNode.y < C) C = tempEndNode.y;
			if (tempEndNode.y > D) D = tempEndNode.y;
		}*/
		OutputImage.create(abs(A - B) + 30, abs(C - D) + 30);
		//cout << "\n" << "S: " << abs(A - B) << ", " << abs(C - D);
		//cout << "\n" << A << ", " << B << ", " << C << ", " << D;
	}
	else { OutputImage.create(10, 10); }

	for (int c = 0; c < comp.size(); c++) {
		sf::Vector2f tempEndNode = comp[c].endNodePos();
		getBounds(comp[c], compBoundArr);

		sf::Image tempCompImg;
		//tempCompImg.createMaskFromColor(sf::Color(23, 24, 25));

		if (comp[c].angle == 0) {
			tempCompImg = compTex[comp[c].serial].copyToImage();
		}
		else if (comp[c].angle == 180) {
			tempCompImg = compTex[comp[c].serial].copyToImage();
			tempCompImg.flipVertically();
			tempCompImg.flipHorizontally();
		}
		else {
			sf::Image tempTempCompImg(compTex[comp[c].serial].copyToImage());
			tempCompImg.create(tempTempCompImg.getSize().y, tempTempCompImg.getSize().x);

			for (int j = 0; j < tempTempCompImg.getSize().y; j++) {
				for (int i = 0; i < tempTempCompImg.getSize().x; i++) {
					tempCompImg.setPixel(j, i, tempTempCompImg.getPixel(i, j));
				}
			}
			tempCompImg.flipHorizontally();


			if (comp[c].angle == 90) {
				;
			}
			else if (comp[c].angle == 270) {
				tempCompImg.flipVertically();
				tempCompImg.flipHorizontally();

			}
		}

		bool rotBool = ((((int)comp[c].angle) / 90) % 2 == 1);

		int fakeGap = 15 - (comp[c].serial == 5) * 5;

		float OffX = compBoundArr[0] - A - !rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;
		float OffY = compBoundArr[2] - C - rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;

		for (int j = rotBool * fakeGap; j < tempCompImg.getSize().y - rotBool * fakeGap; j++) {
			for (int i = !rotBool * fakeGap; i < tempCompImg.getSize().x - !rotBool * fakeGap; i++) {
				if (tempCompImg.getPixel(i, j).a == 0) continue;
				//std::async(std::launch::async, &sf::Image::setPixel, &OutputImage, OffX + i, OffY + j, tempCompImg.getPixel(i, j));

				OutputImage.setPixel(OffX + i, OffY + j, tempCompImg.getPixel(i, j));
			}
		}

	}

	int picNo = 0;
	std::string picDir = "Saved-Images/Untitled-", picType = ".png";
	sf::Image test;

	while (test.loadFromFile(picDir + std::to_string(picNo++) + picType)) {
		;
		//cout << "\n" << picNo;
	}

	OutputImage.saveToFile(picDir + std::to_string(picNo - 1) + picType);
}




float trim(float num, int wrt) {
	return num - (int)num % wrt;
}

sf::Vector2f cursorInSim() {
	return app.mapPixelToCoords(sf::Mouse::getPosition(app));
}

bool Click(int Sensitivity) {
	return !!(!Mouse::isButtonPressed(Mouse::Left) && abs(mouseHoldX - (float)Mouse::getPosition(app).x) <= Sensitivity && abs(mouseHoldY - (float)Mouse::getPosition(app).y) <= Sensitivity);
}

bool occupiedAt(int Index, sf::Vector2f At) {

	int noCount = 0;
	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 100 || abs(comp[c].y - At.y) >= 100) continue;

		if (At.x == comp[c].x && At.y == comp[c].y) {
			if (comp[Index].angle != comp[c].angle) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

		if (At.x == comp[c].endNodePos().x && At.y == comp[c].endNodePos().y) {
			if (abs(comp[Index].angle - comp[c].angle) != 180) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

	}
	if (noCount == 1) { Occupied = 0; return 0; }

	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 100 || abs(comp[c].y - At.y) >= 100) continue;

		int compBoundArr[4];
		getBounds(comp[c], compBoundArr);

		if ((compBoundArr[0] < At.x) && (At.x < compBoundArr[1])) {
			if ((compBoundArr[2] < At.y) && (At.y < compBoundArr[3])) {
				Occupied = 1; return 1;
			}
		}

	}

	Occupied = 0; return 0;
}

bool compIn(Entity Comp, sf::Vector2f Ini, sf::Vector2f Fin) {

	int compBoundArr[4];
	getBounds(Comp, compBoundArr);
	float A = 0, B = 0, C = 0, D = 0;

	if (Ini.x <= Fin.x) { A = Ini.x; B = Fin.x; }
	else { B = Ini.x; A = Fin.x; }

	if (Ini.y <= Fin.y) { C = Ini.y; D = Fin.y; }
	else { D = Ini.y; C = Fin.y; }

	if ((A <= compBoundArr[0]) && (compBoundArr[1] <= B)) {
		if ((C <= compBoundArr[2]) && (compBoundArr[3] <= D)) {
			return 1;
		}
	}

	return 0;
}

void getBounds(Entity Comp, int arr[4]) {

	/*Dealing with Origin*/
	int a = 0, b = 15, d = 75;
	int A = 15, B = 15, C = 0, D = 75, i = (int)Comp.angle % 360;

	if (i == 0) { A = b; B = b; C = a; D = d; }
	else if (i == 90) { A = d; B = a; C = b; D = b; }
	else if (i == 180) { A = b; B = b; C = d; D = a; }
	else if (i == 270) { A = a; B = d; C = b; D = b; }

	arr[0] = Comp.x - A; arr[1] = Comp.x + B; arr[2] = Comp.y - C; arr[3] = Comp.y + D; // Borders

}