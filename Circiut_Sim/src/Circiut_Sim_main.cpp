#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

#include <iostream>
#include <fstream>
//#include <thread>
//#include <future>
//#include <vector>

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

#include "Circuit_Entity.h"
#include "Circuit_wire.h"
//#include "Circuit_Graph.h"

#include "clipboardxx.hpp"

using namespace sf;
using std::cout;

sf::Vector2f cursorInSim();
bool Click(int Sensitivity);
float trim(float num, int wrt);

bool cursorOnComp(const Entity& Comp);
bool occupiedAt(int Index, const sf::Vector2f& At);
bool compIn(const Entity& Comp, const sf::Vector2f& Ini, const sf::Vector2f& Fin);
void getBounds(const Entity& Comp, int arr[4]);

void printScreen();

void updateAllEnds();

//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
sf::RenderWindow app;
sf::View view(sf::Vector2f(W / 2, H / 2), sf::Vector2f(W, H));

bool Occupied = 0;
float mouseHoldX = Mouse::getPosition(app).x, mouseHoldY = Mouse::getPosition(app).y;
time_t click = clock(); // Time passed since Click

std::vector<Entity> comp;
std::vector<sf::Vector2f> allEnds;

//int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
int main() {

	sf::ContextSettings settings; settings.antialiasingLevel = 8;
	app.create(VideoMode(W, H), "CircuitSim",Style::Default, settings);
	//app.create(VideoMode(W, H), "CircuitSim", Style::Default, ContextSettings(0));

	W = app.getSize().x; H = app.getSize().y;
	app.setVerticalSyncEnabled(1);
	app.setFramerateLimit(60);
	srand(time(NULL));

	app.setPosition(sf::Vector2i(450, 100));

	//ImGui::SFML::Init(app);//
	//sf::Clock deltaClock;//

	bool RELEASE_DEBUG = 1;

	time_t frame = clock();
	bool End = 0, debugBool = 0;

	bool stimuliDisplay = 1, stimuliEndNodes = 1;

	bool releaseBool = 1;
	bool ShiftPressed=0;

	bool Drag = 0, selectSquare = 0, mouseOnCompsBool = 0;
	bool wireBool = 0;

	int serialCompMouse = 0, serialToolMouse = 0;

	sf::Color normalCompColor(255, 255, 255), tempDimColor(150, 150, 150);

	///////////////////////////////////////////////

	/*Rayn*/ //Texture t; t.loadFromFile("Images/0 Rayn.png"); Sprite Rayn(t), Rayn2(t); Rayn.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn2.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn.setPosition(app.getSize().x / 2, app.getSize().y / 2); Rayn2.setPosition(app.getSize().x + 10, app.getSize().y + 10);

	/*bool DrawCircle = 1;
	float t_radius = 60, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
	int t_vertices = 34;
	sf::CircleShape testCircle(t_radius, t_vertices);
	testCircle.setOrigin(t_radius, t_radius);
	testCircle.setPosition(W / 2, H / 2);
	testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));*/

	/*Grid*/
	int gap = 15, virtualBoarder = 80;
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;
	sf::Color gridColor(100, 105, 110, 20);
	sf::Color backColor(23, 24, 25);
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
		compTex[Cap].loadFromFile("assets/Images/Cap.png");
		compTex[Cur].loadFromFile("assets/Images/Cur.png");
		compTex[Dod].loadFromFile("assets/Images/Dod.png");
		compTex[Ind].loadFromFile("assets/Images/Ind.png");
		compTex[Res].loadFromFile("assets/Images/Res.png");
		compTex[SwO].loadFromFile("assets/Images/SwO.png");
		compTex[Vol].loadFromFile("assets/Images/Vol.png");
		compTex[SwC].loadFromFile("assets/Images/SwC.png");
	}

	/*Fonts*/
	sf::Font calibriFont;
	calibriFont.loadFromFile("assets/Fonts/CalibriL_1.ttf");/*CALIBRI_1*/
	sf::Font greekFont;
	greekFont.loadFromFile("assets/Fonts/GREEK.ttf");

	////////////////////////////////////////////// ToolBox
	const float c_toolColWidth = 100;
	sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, view.getSize().y));
	float ToolBoxWinRestingPosX = view.getCenter().x - view.getSize().x / 2, ToolBoxWinRestingPosY = view.getCenter().y - view.getSize().y / 2;

	toolCol.setFillColor(sf::Color(0, 0, 0, 120));

	sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(c_toolColWidth, c_toolColWidth));
	float ToolLilWinRestingPosX = view.getCenter().x - view.getSize().x / 2, ToolLilWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
	ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));

	sf::Sprite ToolSpr[7]; //noOfComps-1
	sf::Vector2f ToolSprPOS[7]; //noOfComps-1
	for (int c = 0; c < (noOfComps - 1); c++) {
		ToolSpr[c].setTexture(compTex[c]);
		ToolSpr[c].setOrigin(compTex[c].getSize().x / 2, 0);
		//compSpr[c].setRotation(45);
		ToolSprPOS[c].x = c_toolColWidth / 2;
		ToolSprPOS[c].y = c * c_toolColWidth + (int)(c_toolColWidth - compTex[c].getSize().y) / 2;
		ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);
	}

	////////////////////////////////////////////// VirutalComps
	std::vector<int> virSerial; virSerial.reserve(8);
	std::vector<sf::Sprite> virSprite; virSprite.reserve(8);

	/*Sel Sqr*/
	sf::RectangleShape selSqr; {
		selSqr.setFillColor(sf::Color(66, 135, 245, 60));
		selSqr.setOutlineThickness(1.0f);
		selSqr.setOutlineColor(sf::Color(66, 135, 245)); }

	sf::CircleShape nodePic(4, 15); {
		nodePic.setOrigin(4, 4);
		nodePic.setFillColor(normalCompColor); }
	std::vector<sf::CircleShape> allEndCircles; allEndCircles.reserve(17);

	sf::RectangleShape boarderPic; {
		boarderPic.setFillColor(sf::Color(0, 0, 100, 0));
		boarderPic.setOutlineThickness(1.0f);
		boarderPic.setOutlineColor(sf::Color(0, 204, 102)); }
	std::vector<sf::RectangleShape> allBoarders; allBoarders.reserve(9);

	/*Circuit*/
	comp.reserve(9);
	allEnds.reserve(9);
	//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);

	/*Wires*/
	std::vector<Wire> wires; wires.reserve(3);
	//wires.emplace_back(sf::Vector2f(300, 300));

	////////////////////////////////////////////// Solve
	
	//Graph circuit;

	//std::thread printScreenTread;


	///////////////////////////////////////////////

	clipboardxx::clipboard clipboard;

	float viewX = view.getCenter().x, viewY = view.getCenter().y;
	float verX = vLines[0].getPosition().x, verY = vLines[0].getPosition().y;
	float horX = hLines[0].getPosition().x, horY = hLines[0].getPosition().y;

	int verBrightCount = 5, horBrightCount = 5;

	std::vector<int> virSerialShift; virSerialShift.reserve(9);

	/*Ravi*/ app.setKeyRepeatEnabled(false);
	while (app.isOpen() && !End) {

		W = app.getSize().x; H = app.getSize().y;

		float t_TollWx = toolCol.getPosition().x;
		bool MInTool = !!(0 <= Mouse::getPosition(app).x && Mouse::getPosition(app).x <= c_toolColWidth);
		bool MIntool = !!(MInTool && Mouse::getPosition(app).y < (noOfComps - 1)* c_toolColWidth);
		Occupied = 0;

		Event evnt;
		while (app.pollEvent(evnt)) {
			stimuliDisplay = 1;
			//ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == evnt.Closed) { app.close(); End = 1; }
			if (evnt.type == evnt.Resized) {

				//view = sf::View(sf::FloatRect(view.getCenter().x - ((float)evnt.size.width) / 2, view.getCenter().y - ((float)evnt.size.height) / 2, evnt.size.width, evnt.size.height));
				view.setCenter(view.getCenter()); view.setSize(evnt.size.width, evnt.size.height);

				/*Resting Pos of Tool Bar   (Repeated at the at of Drag{}) */ {
					toolCol.setSize(sf::Vector2f(toolCol.getSize().x, view.getSize().y));
					ToolBoxWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolBoxWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					ToolLilWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolLilWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					for (int c = 0; c < (noOfComps - 1); c++) ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);
				}
			}

			if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == Mouse::Left) {
				cout << "\nPressed";

				bool onNode = 0;
				static const int sensitivity = 7;
				sf::Vector2f cursorPos = cursorInSim();

				int e = 0;
				for (; e < allEnds.size(); e++) {
					if (abs(cursorPos.x - allEnds[e].x) < sensitivity && abs(cursorPos.y - allEnds[e].y) < sensitivity) { onNode = 1; break; }
				}

				if (onNode) {
					cout << ", On Node";

					if (!wireBool)wires.emplace_back(allEnds[e]);

					wireBool = !wireBool;
				}
				else {

					if(wireBool) wires.back().newEdge();
					
				}

			}
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == Mouse::Left) { releaseBool = 1; }

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == Keyboard::Escape) { app.close(); End = 1; cout << "\n------------------ESC Pressed-----------------\n"; continue; }
				if (evnt.key.code == Keyboard::Delete) {
					stimuliDisplay = 1;	stimuliEndNodes = 1;

					while (0 < virSerial.size()) {
						if (comp.size() > 0) comp.erase(comp.begin() + virSerial[0]);
						if (virSprite.size() > 0) virSprite.erase(virSprite.begin());

						for (int c = 1; c < virSerial.size(); c++) {
							if (virSerial[c] > virSerial[0]) virSerial[c]--;
						}

						virSerial.erase(virSerial.begin());
					}
				}
				if (evnt.key.code == Keyboard::W) { wireBool = !wireBool; /*cout << "\ndebug\n";*/ }
				//if (evnt.key.code == Keyboard::N) { debugBool = !debugBool; /*cout << "\ndebug\n";*/ }
				//if (evnt.key.code == Keyboard::P) { printScreenBool = 1; }
				//if (evnt.key.code == Keyboard::S) { save = 1; }
				//if (evnt.key.code == Keyboard::O) { open = 1; }

				/*Ctrl*/
				if (evnt.key.control) {
					if (evnt.key.code == Keyboard::A) {
						cout << "\nCtrl + A\n";

						while (comp.size() < virSerial.size()) {
							virSerial.pop_back();
							virSprite.pop_back();
						}

						for (int v = 0; v < virSerial.size(); v++) {
							virSerial[v] = v;
							virSprite[v] = comp[virSerial[v]].sprite;
						}

						for (int c = virSerial.size(); c < comp.size(); c++) {
							virSerial.emplace_back(c);
							virSprite.emplace_back(comp[virSerial.back()].sprite);
						}

					}
					if (evnt.key.code == Keyboard::R) {
						cout << "\nCtrl + R\n"; stimuliDisplay = 1;	stimuliEndNodes = 1;

						for (int v = 0; v < virSerial.size(); v++) {
							comp[virSerial[v]].angle += 90;
							comp[virSerial[v]].angle -= (int)(comp[virSerial[v]].angle / 360) * 360;
						}
					}
					if (evnt.key.code == Keyboard::O) {
						cout << "\nCtrl + O\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

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
					}
					if (evnt.key.code == Keyboard::S) {
						cout << "\nCtrl + S\n";

						std::ofstream output;

						int fileNo = 0;
						std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
						std::ifstream test(fileDir + std::to_string(fileNo) + fileType);

						while (test.good()) {
							test.close();
							test.open(fileDir + std::to_string(++fileNo) + fileType);
						}
						output.open(fileDir + std::to_string(fileNo) + fileType);

						std::string tempStr;
						tempStr = std::to_string((int)comp.size()) + "\n";
						int size = (int)comp.size();
						for (int c = 0; c < size; c++) {
							tempStr += std::to_string(comp[c].serial) + "\t" + std::to_string((int)comp[c].x) + "\t" + std::to_string((int)comp[c].y) + "\t" + std::to_string((int)comp[c].angle) + "\n";
						}
						output << tempStr;
						output.close();
					}
					if (evnt.key.code == Keyboard::C) {
						cout << "\nCtrl + C\n";

						std::ofstream output;
						std::string tempStr(std::to_string((int)virSerial.size()) + "\n");

						int size = (int)virSerial.size();
						for (int c = 0; c < size; c++) {
							tempStr += std::to_string(comp[virSerial[c]].serial) + "\t" + std::to_string((int)comp[virSerial[c]].x) + "\t" + std::to_string((int)comp[virSerial[c]].y) + "\t" + std::to_string((int)comp[virSerial[c]].angle) + "\n";
						}

						clipboard << tempStr;
					}
					if (evnt.key.code == Keyboard::X) {
						cout << "\nCtrl + X\n"; stimuliDisplay = 1;	stimuliEndNodes = 1;

						// Copy
						{
							std::ofstream output;
							std::string tempStr(std::to_string((int)virSerial.size()) + "\n");

							for (int v = 0; v < virSerial.size(); v++) {
								tempStr += std::to_string(comp[virSerial[v]].serial) + "\t" + std::to_string((int)comp[virSerial[v]].x) + "\t" + std::to_string((int)comp[virSerial[v]].y) + "\t" + std::to_string((int)comp[virSerial[v]].angle) + "\n";
							}

							clipboard << tempStr;
						}
						
						// Delete
						{
							while (0 < virSerial.size()) {
								if (comp.size() > 0) comp.erase(comp.begin() + virSerial[0]);
								if (virSprite.size() > 0) virSprite.erase(virSprite.begin());

								for (int v = 1; v < virSerial.size(); v++) {
									if (virSerial[v] > virSerial[0]) virSerial[v]--;
								}

								virSerial.erase(virSerial.begin());
							}
						}
					}
					if (evnt.key.code == Keyboard::V) {
						cout << "\nCtrl + V\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

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
					}
				}
			}
			if (GetAsyncKeyState(VK_SNAPSHOT)) {
				cout << "\nPrintScreen";

				//time_t print = clock();

				/*sf::Texture tex;

				tex.update(app);

				sf::Image screenshot(tex.copyToImage());

				screenshot.saveToFile("screenshot.png");*/

				//screenshot = app.capture();
				//screenshot.saveToFile("screenshot.png");

				for (int c = 0; c < 1; c++) {
					//std::thread printScreenTread { printScreen }; printScreenTread.join();

					//std::async(std::launch::async, printScreen);

					printScreen();
				}
				//cout << "\n" << ((float)clock() - (float)print) / (float)CLOCKS_PER_SEC;
			}

			/*int difr = 10;
				if (evnt.key.code == Keyboard::Up) { view.setCenter(view.getCenter().x, view.getCenter().y - difr); }
				if (evnt.key.code == Keyboard::Down) { view.setCenter(view.getCenter().x, view.getCenter().y + difr); }
				if (evnt.key.code == Keyboard::Right) { view.setCenter(view.getCenter().x + difr, view.getCenter().y); }
				if (evnt.key.code == Keyboard::Left) { view.setCenter(view.getCenter().x - difr, view.getCenter().y); }*/
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
		ShiftPressed = (Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift));

		///////////////////////////////////////////////


		








		// ----------------------------------------	Options
		{
			/*Mouse Hold*/
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (releaseBool) {
					releaseBool = 0;
					click = clock();
					mouseHoldX = (float)Mouse::getPosition(app).x; mouseHoldY = (float)Mouse::getPosition(app).y;

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

							if (cursorOnComp(comp[c])) {

								mouseOnCompsBool = 1; Drag = 0;

								if (!ShiftPressed) {
									virSerial.clear();
									virSprite.clear();
								}

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

						//////////// Urgent need of enums , State Machine
						if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; selSqr.setPosition(cursorInSim()); }

						/*Wheel*/
						/*Drag Background*/
						/*if (!mouseOnCompsBool && !selectSquare) {
							Drag = 1; mouseOnCompsBool = 0;
							viewX = view.getCenter().x, viewY = view.getCenter().y;
							verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
							horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
						}*/
					}
				}

				if (virSerial.size() > 0) mouseOnCompsBool = 1;

				/*Once while hold*/
				/*if (mouseOnCompsBool && rotComp) {
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
				else onceOptMounsePressed = 1;*/
			}
			else {
				/*Wheel*//*Drag = 0;*/ mouseOnCompsBool = 0; selectSquare = 0;

				if (0 && ShiftPressed) {
					virSerialShift = virSerial;


					/*for (int vs = 0; vs < virSerialShift.size(); vs++) {
						static bool found = 0;
						for (int v = 0; v < virSerial.size(); v++) {
							if (virSerial[v] == virSerialShift[vs]) {
								found = 1;
								break;
							}
						}
						if (!found) {
							virSerial.emplace_back(virSerialShift[vs]);
							virSprite.emplace_back(comp[virSerial.back()].sprite);
						}
					}*/


				}
				/*Click*/
				else {
					if (Click(gap)) {
						stimuliDisplay = 1;

						for (int v = 0; v < virSerial.size(); v++) {
							if (cursorOnComp(comp[virSerial[v]])) {

								if (comp[virSerial[v]].serial == 5)comp[virSerial[v]].serial = 7;
								else if (comp[virSerial[v]].serial == 7) comp[virSerial[v]].serial = 5;

								comp[virSerial[v]].sprite.setTexture(compTex[comp[virSerial[v]].serial]);

								virSerial.erase(virSerial.begin() + v);
								virSprite.erase(virSprite.begin() + v);

								break;
							}

						}

					}

					if (Click(3)) {
						virSprite.clear();  ///// 
						virSerial.clear();  ///// 
					}
				}
				/* //ZZzzzz Recolor back to normal    & clear serials
				if (virSerial.size() != 0) {
					for (int v = 0; v < virSerial.size() && comp.size() != 0; v++) {
						//cout << "\nChanging" << virSerial[v];
						comp[virSerial[v]].sprite.setColor(normalCompColor);
					}
				}*/
			}

			/*Wheel*/
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
				/*Drag Background*/
				if (releaseBool /*!mouseOnCompsBool && !selectSquare*/) {
					mouseHoldX = (float)Mouse::getPosition(app).x; mouseHoldY = (float)Mouse::getPosition(app).y;
					Drag = 1; mouseOnCompsBool = 0;
					viewX = view.getCenter().x, viewY = view.getCenter().y;
					verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
					horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
				}
				releaseBool = 0;
			}
			else { Drag = 0; }

			/*Continoue while hold*/
			if (mouseHoldX != (float)Mouse::getPosition(app).x || mouseHoldY != (float)Mouse::getPosition(app).y) {

				/*Follow Mouse*/
				if (mouseOnCompsBool) {
					if (!selectSquare /*&& releaseBool*/) {
						int offSet[4][2] = {
							{0, -2},
							{2, 0},
							{0, 2},
							{-2, 0}
						};
						for (int c = 0; c < virSerial.size(); c++) {

							float tempX = (int)cursorInSim().x + gap * offSet[(int)comp[virSerial[c]].angle / 90][0]; ///
							float tempY = (int)cursorInSim().y + gap * offSet[(int)comp[virSerial[c]].angle / 90][1]; ///

							//float tempX = cursorInSim().x - mouseHoldX + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
							//float tempY = cursorInSim().y - mouseHoldY + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///

							//for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }
							for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX, tempY); }

							tempX = trim(tempX, gap);
							tempY = trim(tempY, gap);

							if (!occupiedAt(virSerial[c], sf::Vector2f(tempX, tempY))) {
								comp[virSerial[c]].x = (int)tempX; // += (were "=" before)
								comp[virSerial[c]].y = (int)tempY; // += (were "=" before)
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
					}

					//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));

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
				if (0 && ShiftPressed) {
					for (int vs = 0; vs < virSerialShift.size(); vs++) {
						static bool found = 0;
						for (int v = 0; v < virSerial.size(); v++) {
							if (virSerial[v] == virSerialShift[vs]) {
								found = 1;
								break;
							}
						}
						if (!found) {
							virSerial.emplace_back(virSerialShift[vs]);
							virSprite.emplace_back(comp[virSerial.back()].sprite);
						}
					}
				}

			}

			/*Wire*/
			if (wireBool) { stimuliDisplay = 1; wires.back().makeWire(app); }
		}

		//cout << "\n" << virSerialShift.size();

		// ----------------------------------------	Update

		//circuit.updateWin();
		if (MInTool) { stimuliDisplay = 1; }
		{
			/*ImGui*/
			/*
			ImGui::SFML::Update(app, deltaClock.restart());
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
			testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));
			//*/

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
			toolCol.setPosition((MInTool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (noOfComps - 1)) + (!MInTool) * (t_TollWx + (ToolBoxWinRestingPosX - c_toolColWidth - t_TollWx) / (noOfComps - 1)), ToolBoxWinRestingPosY);

			//Tool Sqr
			t_TollWx = ToolBoxLittleBox.getPosition().x;
			ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (noOfComps - 1)) + (!MIntool) * (t_TollWx + (ToolBoxWinRestingPosX - c_toolColWidth - t_TollWx) / (noOfComps - 1)), ToolLilWinRestingPosY + trim(Mouse::getPosition(app).y, c_toolColWidth));
			if (MIntool) serialToolMouse = (int)(Mouse::getPosition(app).y / c_toolColWidth); else serialToolMouse = 0;

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

			if (RELEASE_DEBUG/* && stimuliEndNodes*/) {

				while (virSerial.size() < allBoarders.size()) {
					allBoarders.pop_back();
				}

				while (allBoarders.size() < virSerial.size()) {
					allBoarders.emplace_back(boarderPic);
				}

				for (int v = 0; v < virSerial.size(); v++) {
					static int bounds[4] = { 0,0,0,0 };
					getBounds(comp[virSerial[v]], bounds);
					allBoarders[v].setPosition(bounds[0], bounds[2]);
					allBoarders[v].setSize(sf::Vector2f(bounds[1] - bounds[0], bounds[3] - bounds[2]));
					//cout << "\n\nPos: " << allRect[v].getSize().x << ", " << allRect[v].getSize().y;
				}

				//cout << "\nallBoarders.size() = " << allBoarders.size();
			}

			//testCircle.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 + 200);
		}

		
		// ----------------------------------------	Draw
		if (1 || stimuliDisplay) { // zero causes 100 cpu load
			app.setView(view);
			app.clear(backColor);
			//app.draw(Rayn);
			//app.draw(Rayn2);

			/*grid*/ {
				for (int c = 0; c < vLines.size(); c++) { app.draw(vLines[c]); }
				for (int c = 0; c < hLines.size(); c++) { app.draw(hLines[c]); }
			}

			/*comp*/ {
				for (int c = 0; c < comp.size(); c++) { comp[c].draw(app); }
			}

			/*Wires*/ for (int c = 0; c < wires.size(); c++) wires[c].draw(app);

			/*Nodes*/ {
				for (int e = 0; e < allEndCircles.size(); e++) { app.draw(allEndCircles[e]); }
			}

			/*Boarders*/ {
				for (int b = 0; b < allBoarders.size(); b++) { app.draw(allBoarders[b]); }
			}

			//if (Occupied) for (int v = 0; v < virSprite.size(); v++) { app.draw(virSprite[v]); }

			//if (DrawCircle) app.draw(testCircle);

			if (selectSquare && !releaseBool) app.draw(selSqr);

			/*Tool Win*/ {
				if (MInTool) {
					app.draw(toolCol);
					for (int c = 0; c < (noOfComps - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
					for (int c = 0; c < (noOfComps - 1); c++) { app.draw(ToolSpr[c]); }
				}
				else { for (int c = 0; c < (noOfComps - 1); c++) ToolSpr[c].setColor(sf::Color(255, 255, 255, 0)); }

				if (MIntool) app.draw(ToolBoxLittleBox);
			}


			/*ImGui*/
			//ImGui::SFML::Render(app);//Last Thing to render

			app.display();
		}

		app.setTitle("CircuitSIm   " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
		frame = clock();

		stimuliDisplay = 0; stimuliEndNodes = 0;
	}
	


	stimuliDisplay = 1; stimuliEndNodes = 1;
	
	/*ImGui*/
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
	return (((float)clock() - (float)click) < 100) && !!(!Mouse::isButtonPressed(Mouse::Left) && abs(mouseHoldX - (float)Mouse::getPosition(app).x) <= Sensitivity && abs(mouseHoldY - (float)Mouse::getPosition(app).y) <= Sensitivity);
}

bool cursorOnComp(const Entity& Comp) {

	int bounds[4];
	getBounds(Comp, bounds);

	sf::Vector2f cursorPos = cursorInSim();

	if (bounds[0] <= cursorPos.x && cursorPos.x <= bounds[1] && bounds[2] <= cursorPos.y && cursorPos.y <=bounds[3]) return 1;

	return 0;
}

bool occupiedAt(int Index, const sf::Vector2f& At) {

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

bool compIn(const Entity& Comp, const sf::Vector2f& Ini, const sf::Vector2f& Fin) {

	int compBoundArr[4];
	getBounds(Comp, compBoundArr);
	float A = 0, B = 0, C = 0, D = 0;

	if (Ini.x <= Fin.x) { A = Ini.x; B = Fin.x; }
	else { B = Ini.x; A = Fin.x; }

	if (Ini.y <= Fin.y) { C = Ini.y; D = Fin.y; }
	else { D = Ini.y; C = Fin.y; }

	if ((A <= compBoundArr[0]) && (compBoundArr[1] <= B) && (C <= compBoundArr[2]) && (compBoundArr[3] <= D)) {
			return 1;
	}

	return 0;
}

void getBounds(const Entity& Comp, int arr[4]) {

	/*Dealing with Origin*/
	int a = 0, b = 15, d = 75;
	int A = 15, B = 15, C = 0, D = 75, i = (int)Comp.angle % 360;

	if (i == 0) { A = b; B = b; C = a; D = d; }
	else if (i == 90) { A = d; B = a; C = b; D = b; }
	else if (i == 180) { A = b; B = b; C = d; D = a; }
	else if (i == 270) { A = a; B = d; C = b; D = b; }

	arr[0] = Comp.x - A; arr[1] = Comp.x + B; arr[2] = Comp.y - C; arr[3] = Comp.y + D; // Borders

}