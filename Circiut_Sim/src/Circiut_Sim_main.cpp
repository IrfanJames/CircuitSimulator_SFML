// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
/*
taskkill /F /IM Circiut_Sim.exe
*/
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <direct.h>
//#include <thread>
//#include <future>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Circuit_GUI.hpp"
#include "SFML/Graphics.hpp"
#include "Circuit_Entity.hpp"
#include "Circuit_Windows_Stuff.hpp"
#include "Resource_Manager.hpp"
//#include "Circuit_Graph.h"
//asdf#include "Circuit_wire.h"

using std::cout;
using namespace CircuitGUI;

#ifdef _DEBUG
// Debug  Mode
int main(int argc, char* argv[]) {
#else
// Realse Mode
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif


#ifdef NDEBUG
	// Release
	std::remove("log.txt");
	std::ofstream LOG("log.txt");
	LOG << lpCmdLine;
#endif

	CircuitGUI::initializeGUI();

	/*ImGui*/
	ImGui::SFML::Init(CircuitGUI::app);
	sf::Clock deltaClock;

	srand(time(NULL));
	time_t FrameTime_for_FPS = clock();
	bool End = 0, debugBool = 0;
	bool Drag = 0, stimuliDisplay = 1, stimuliEndNodes = 0;
	bool releaseBool = 1, wheelReleaseBool = 1, ShiftPressed = 0;
	bool selectSquare = 0, Selection = 0, mouseOnCompsBool = 0, wireBool = 0;
	bool PlayMode = 0/*//asdf, PlayRot = 0*/;

	int serialCompMouse = 0, serialToolMouse = 0, cursorInWin = 0;

	/* Test Circle
	bool DrawCircle = 1; int t_vertices = 34; float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 }; sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/

	/*Wires*/
	//asdfstd::vector<Wire> wires; wires.reserve(3);
		//std::thread printScreenTread;

	////////////////// Solve //////////////////////
		//Graph circuit;
		///////////////////////////////////////////////


	////////////////// argv ///////////////////////
#ifdef _DEBUG
	if (1 < argc) {
		std::string temp(argv[1]);
		if (!temp.empty())
			CircuitGUI::Options::openf(temp);

		stimuliEndNodes = 1; //CircuitGUI::updateAllSqr();
	}
#else
	/*
	std::istringstream iss(lpCmdLine);
	std::string arg;
	if (std::getline(iss, arg, ' '))
	{
		LOG << "\n" << arg << "\n";
		if (!arg.empty())
			CircuitGUI::Options::openf(arg);

		stimuliEndNodes = 1; //CircuitGUI::updateAllSqr();
	}//*/

	//* // Multiple Files
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	for (int i = 1; i < argc; i++) {
		std::wstring temp(argv[i]);
		if (!temp.empty())
		{
			LOG << "Dropped file: " << std::string(temp.begin(), temp.end()) << "\n";
			CircuitGUI::Options::openf(std::string(temp.begin(), temp.end()).c_str());
			stimuliEndNodes = 1; //CircuitGUI::updateAllSqr();
		}
	}
	LocalFree(argv);//*/
#endif


	//////////////// Main Loop ////////////////////
	while (CircuitGUI::app.isOpen() && !End) {

		while (CircuitGUI::app.pollEvent(CircuitGUI::evnt)) {
			using CircuitGUI::evnt;
			stimuliDisplay = 1; /*cout << "1";*/
			/*ImGui*/
			ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == evnt.Closed) { CircuitGUI::app.close(); End = 1; }
			if (evnt.type == evnt.MouseLeft) { cursorInWin = 0; }
			if (evnt.type == evnt.MouseEntered) { cursorInWin = 1; }
			if (evnt.type == evnt.Resized) {
				W = CircuitGUI::app.getSize().x; H = CircuitGUI::app.getSize().y;

				view = sf::View(sf::FloatRect((int)view.getCenter().x - (int)(evnt.size.width / 2), (int)view.getCenter().y - (int)(evnt.size.height / 2), (int)evnt.size.width, (int)evnt.size.height));
				//CircuitGUI::view.setSize((int)evnt.size.width, (int)evnt.size.height);
				//CircuitGUI::view.setCenter((int)CircuitGUI::view.getCenter().x, (int)CircuitGUI::view.getCenter().y);

				CircuitGUI::toolCol.setSize(sf::Vector2f(CircuitGUI::toolCol.getSize().x, CircuitGUI::view.getSize().y));
				CircuitGUI::updatePosToolBox();
			}

			//asdf
			/*if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left) {
				bool onNode = 0;
				static int sensitivity = 7;
				sf::Vector2f cursorPos = CircuitGUI::cursorInSim();

				sf::Vector2f start(0, 0), stop(0, 0);
				if (comp.size() > 2) start = { comp[0].x, comp[0].y }, stop = { comp[1].x, comp[1].y };


				int e = 0;
				if (PlayMode) {
					sensitivity = 30;
					if (abs(cursorPos.x - start.x) < sensitivity && abs(cursorPos.y - start.y) < sensitivity) { onNode = 1; e = 1; }
					else if (abs(cursorPos.x - stop.x) < sensitivity && abs(cursorPos.y - stop.y) < sensitivity) { onNode = 1; e = 2; }
				}
				else {
					for (; e < CircuitGUI::allEnds.size(); e++) {
						if (abs(cursorPos.x - CircuitGUI::allEnds[e].x) < sensitivity && abs(cursorPos.y - CircuitGUI::allEnds[e].y) < sensitivity) { onNode = 1; break; }
					}
				}

				if (onNode) {

					if (PlayMode)
					{
						if (!wireBool) {
							if (e == 1) {
								PlayRot = 1; wires.emplace_back(start); wireBool = 1;
							}
						}
						else if (e == 2) { PlayRot = 0; PlayMode = 0; wireBool = 0; }

					}
					else
					{
						cout << ", On Node";

						if (!wireBool) {
							if (PlayMode) PlayRot = 1;
							wires.emplace_back(CircuitGUI::allEnds[e]);
						}

						wireBool = !wireBool;

					}


				}
				else {

					if (wireBool) {
						wires.back().newEdge();
					}

				}

			}*/
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left) {
				releaseBool = 1;
				if (Selection) {
					Selection = 0;

					//virSprite.clear();
					virSprite.reserve(virSerial.size());
					/*for (int v = 0; v < virSerial.size(); v++) {
						virSprite.emplace_back(comp[virSerial[v]].sprite);
						virSprite.back().setColor(CircuitGUI::tempDimColor); }*/


					for (int v = 0; v < virSerial.size() && v < virSprite.size(); v++) {
						virSprite[v] = comp[virSerial[v]].sprite;
						virSprite[v].setColor(CircuitGUI::tempDimColor);
					}

					if (virSerial.size() < virSprite.size())
					{
						virSprite.erase(virSprite.begin() + (int)virSerial.size(), virSprite.end());
					}

					if (virSerial.size() > virSprite.size())
					{
						for (int v = virSprite.size(); v < virSerial.size(); v++) {
							virSprite.emplace_back(comp[virSerial[v]].sprite);
							virSprite.back().setColor(CircuitGUI::tempDimColor);
						}

					}

				}
			}
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Middle) { wheelReleaseBool = 1; }

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == sf::Keyboard::Escape) { CircuitGUI::app.close(); End = 1; continue; }
				if (evnt.key.code == sf::Keyboard::Delete) {
					stimuliDisplay = 1; /*cout << "2";*/	stimuliEndNodes = 1;
					CircuitGUI::Options::deletef();
				}
				//if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool;}

				/*Ctrl*/
				if (evnt.key.control) {
					if (evnt.key.code == sf::Keyboard::A) {
						stimuliDisplay = 1; /*cout << "3";*/

						while (CircuitGUI::comp.size() < CircuitGUI::virSerial.size()) {
							CircuitGUI::virSerial.pop_back();
							CircuitGUI::virSprite.pop_back();
						}

						for (int v = 0; v < CircuitGUI::virSerial.size(); v++) {
							CircuitGUI::virSerial[v] = v;
							CircuitGUI::virSprite[v] = CircuitGUI::comp[CircuitGUI::virSerial[v]].sprite;
						}

						for (int c = CircuitGUI::virSerial.size(); c < CircuitGUI::comp.size(); c++) {
							CircuitGUI::virSerial.emplace_back(c);
							CircuitGUI::virSprite.emplace_back(CircuitGUI::comp[CircuitGUI::virSerial.back()].sprite);
						}

						CircuitGUI::updateAllSqr();
					}
					if (evnt.key.code == sf::Keyboard::R) {
						stimuliDisplay = 1;	/*cout << "4";*/ stimuliEndNodes = 1;

						CircuitGUI::Options::rotatef();

					}
					if (evnt.key.code == sf::Keyboard::C) {

						CircuitGUI::Options::copyf();

					}
					if (evnt.key.code == sf::Keyboard::X) {
						stimuliDisplay = 1;	/*cout << "6";*/ stimuliEndNodes = 1;

						// Copy
						{
							CircuitGUI::Options::copyf();
						}

						// Delete
						{
							CircuitGUI::Options::deletef();
						}
					}
					if (evnt.key.code == sf::Keyboard::V) {
						stimuliDisplay = 1; /*cout << "7";*/ stimuliEndNodes = 1;

						CircuitGUI::Options::pastef();

					}
					if (evnt.key.code == sf::Keyboard::O) {
						stimuliDisplay = 1; /*cout << "5";*/ stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (evnt.key.code == sf::Keyboard::S) {
						;
						//_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
						//_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
					}
					if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {

						_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
						_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty()) {

							if (filepath.back() == 'T')
								CircuitGUI::Options::savef(filepath);

							if (filepath.back() == 'G')
								CircuitGUI::Options::printScreen(filepath);

						}

					}
				}
			}


			/*int difr = 10;
			if (evnt.key.code == sf::Keyboard::Up)		{ CircuitGUI::view.move(0, -difr); }
			if (evnt.key.code == sf::Keyboard::Down)	{ CircuitGUI::view.move(0, difr); }
			if (evnt.key.code == sf::Keyboard::Right)	{ CircuitGUI::view.move(difr, 0); }
			if (evnt.key.code == sf::Keyboard::Left)	{ CircuitGUI::view.move(-difr, 0); }*/

		}
		ShiftPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
		bool MInTool = !!(cursorInWin && sf::Mouse::getPosition(CircuitGUI::app).x <= CircuitGUI::c_toolColWidth);
		bool MIntool = !!(cursorInWin && MInTool && sf::Mouse::getPosition(CircuitGUI::app).y < (Entity::noOfComps - 1) * CircuitGUI::c_toolColWidth);
		float t_TollWx = CircuitGUI::toolCol.getPosition().x;
		//int myWinState = CircuitGUI::Response::State::None;

		//--------------------------------------------------------------------------------//





		// ----------------------------------------	Options
		{
			/*sf::Mouse Hold*/
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { //////////// Urgent need of enums , State Machine
				if (releaseBool) {
					releaseBool = 0;
					CircuitGUI::click = clock();
					CircuitGUI::mouseHoldX = (float)sf::Mouse::getPosition(CircuitGUI::app).x; CircuitGUI::mouseHoldY = (float)sf::Mouse::getPosition(CircuitGUI::app).y;

					/*new Comp*/
					if (MIntool) {
						MIntool = 0;
						float tempNewCompX = 150 + CircuitGUI::view.getCenter().x - W / 2, tempNewCompY = 150 + CircuitGUI::view.getCenter().y - H / 2;
						tempNewCompX = CircuitGUI::trim(tempNewCompX);
						tempNewCompY = CircuitGUI::trim(tempNewCompY);

						CircuitGUI::comp.emplace_back(serialToolMouse, tempNewCompX, tempNewCompY, 0);
						CircuitGUI::comp.back().stimuli();

						/*Collisions*/
						tempNewCompX = CircuitGUI::trim(tempNewCompX, CircuitGUI::gap);
						while (CircuitGUI::occupiedAt(CircuitGUI::comp.size() - 1, sf::Vector2f(tempNewCompX, tempNewCompY))) {
							tempNewCompX = CircuitGUI::trim(tempNewCompX + 6 * CircuitGUI::gap);

							if (tempNewCompX + 7 * CircuitGUI::gap - 150 - CircuitGUI::view.getCenter().x + W / 2 + 91 >= W) {
								tempNewCompX = 150 + CircuitGUI::view.getCenter().x - W / 2;
								tempNewCompY = CircuitGUI::trim(tempNewCompY + 6 * CircuitGUI::gap);
							}
						}

						CircuitGUI::comp.back().x = (int)trim(tempNewCompX); CircuitGUI::comp.back().y = (int)trim(tempNewCompY);
						CircuitGUI::comp.back().stimuli();

						stimuliEndNodes = 1;

					}
					else {
						mouseOnCompsBool = 0;

						if (CircuitGUI::virSerial.size() > 0 && CircuitGUI::allSqr.getGlobalBounds().contains(CircuitGUI::cursorInSim())) mouseOnCompsBool = 1;
						else {

							/*Check every component for Mouse*/
							for (int c = 0; /*!mouseOnCompsBool &&*/ c < CircuitGUI::comp.size(); c++) {

								if (CircuitGUI::comp[c].bounds.contains(CircuitGUI::cursorInSim())) {

									mouseOnCompsBool = 1; Drag = 0;

									if (!ShiftPressed) {
										CircuitGUI::virSerial.clear();
										CircuitGUI::virSprite.clear();
										CircuitGUI::virSerialShift.clear();
									}
									CircuitGUI::virSerial.emplace_back(c);

									//copied in sel square selection
									CircuitGUI::virSprite.emplace_back(CircuitGUI::comp[CircuitGUI::virSerial.back()].sprite);
									CircuitGUI::virSprite.back().setOrigin(CircuitGUI::virSprite.back().getTexture()->getSize().x / 2, CircuitGUI::virSprite.back().getTexture()->getSize().y / 2);
									CircuitGUI::virSprite.back().setColor(CircuitGUI::tempDimColor);/*
									for (int v = 0; v < virSerial.size(); v++) {//copied in sel square selection
										virSprite.emplace_back(comp[virSerial[v]].sprite);
										virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
										virSprite.back().setColor(tempDimColor);
									}*/

									break;
								}
							}
							CircuitGUI::updateAllSqr();

							/*SelSqr*/
							if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; CircuitGUI::selSqr.setPosition(CircuitGUI::cursorInSim()); }
						}

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
			}
			else {
				/*Wheel*//*Drag = 0;*/ mouseOnCompsBool = 0; selectSquare = 0;

				if (0 && ShiftPressed) {
					CircuitGUI::virSerialShift = CircuitGUI::virSerial;

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
					if (/*//asdf!PlayMode &&*/CircuitGUI::Click(CircuitGUI::gap)) {
						stimuliDisplay = 1; /*cout << "8";*/

						// Open/Close the Switch
						using namespace CircuitGUI;
						for (int v = 0; v < virSerial.size(); v++) {
							if (comp[virSerial[v]].bounds.contains(cursorInSim())) {

								// Changing Serial
								if (comp[virSerial[v]].serial == Entity::SwC)
									comp[virSerial[v]].serial = Entity::SwO;
								else if (comp[virSerial[v]].serial == Entity::SwO)
									comp[virSerial[v]].serial = Entity::SwC;

								// Changing Texture
								comp[virSerial[v]].sprite.setTexture(compTex[comp[virSerial[v]].serial]);


								virSerial.erase(virSerial.begin() + v);
								virSprite.erase(virSprite.begin() + v);

								break;
							}

						}

					}

					if (CircuitGUI::Click(3)) {
						CircuitGUI::virSprite.clear();  ///// 
						CircuitGUI::virSerial.clear();  ///// 
					}
				}
			}

			/*Wheel*/
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)/*myWinState & CircuitGUI::Response::Mouse_Drag*/) {

				/*Drag Background*/
				if (wheelReleaseBool /*!mouseOnCompsBool && !selectSquare*/) {
					Drag = 1;
					mouseOnCompsBool = 0;
					CircuitGUI::iniDrag();
				}
				wheelReleaseBool = 0;
			}
			else { Drag = 0; }

			/*Continoue while hold*/
			if (!selectSquare && mouseOnCompsBool /*//asdf&& !PlayMode*/ && !Click(0) /*&& releaseBool*/) {
				/*static const sf::Vector2f offsetHold[4] = {
					{0, -2},
					{2, 0},
					{0, 2},
					{-2, 0}
				};*/
				sf::FloatRect virArea = allSqr.getGlobalBounds();
				sf::Vector2f offsetPos((int)CircuitGUI::cursorInSim().x - (virArea.left + (int)(virArea.width / 2)), (int)CircuitGUI::cursorInSim().y - (virArea.top + (int)(virArea.height / 2)));

				for (int v = 0; v < CircuitGUI::virSprite.size(); v++)
					CircuitGUI::virSprite[v].setPosition(offsetPos.x + comp[virSerial[v]].x, offsetPos.y + comp[virSerial[v]].y);

				offsetPos = CircuitGUI::trim(offsetPos);

				bool moveAll = 1;
				sf::Vector2f temp;
				for (int v = 0; v < CircuitGUI::virSerial.size(); v++) {
					temp.x = offsetPos.x + CircuitGUI::comp[CircuitGUI::virSerial[v]].x;
					temp.y = offsetPos.y + CircuitGUI::comp[CircuitGUI::virSerial[v]].y;

					if (CircuitGUI::occupiedAt(CircuitGUI::virSerial[v], temp, true)) { moveAll = 0; break; }
				}
				if (moveAll) {
					for (int v = 0; v < CircuitGUI::virSerial.size(); v++) {
						CircuitGUI::comp[CircuitGUI::virSerial[v]].x += (int)offsetPos.x; // += (were "=" before)
						CircuitGUI::comp[CircuitGUI::virSerial[v]].y += (int)offsetPos.y; // += (were "=" before)
						CircuitGUI::comp[CircuitGUI::virSerial[v]].stimuli();
					}
				}

				stimuliEndNodes = 1; stimuliDisplay = 1; /*cout << "9";*/
			}

			/*Select Sqr*/
			if (selectSquare && !releaseBool /*//asdf&& !PlayMode*/) {
				stimuliDisplay = 1; /*cout << "10";*/
				Selection = 1;
				/*Sel Sqr*/
				CircuitGUI::selSqr.setSize(CircuitGUI::cursorInSim() - CircuitGUI::selSqr.getPosition());


				/*Selection*/
				for (int c = 0; c < CircuitGUI::comp.size(); c++) {
					//cout << std::count(virSerial.begin(), virSerial.end(), c);
					//bool bi = std::binary_search(virSerial.begin(), virSerial.end(), c);
					//auto mk = std::find(virSerial.begin(), virSerial.end(), c);
					int v = lower_bound(virSerial.begin(), virSerial.end(), c) - virSerial.begin();
					bool compFound = 0;
					if (v < virSerial.size()) compFound = (virSerial[v] == c);
					//bool compFound = 0;
					//int v = 0;
					//for (; v < CircuitGUI::virSerial.size() && c >= CircuitGUI::virSerial[v]; v++) { // if(CircuitGUI::virSerial[v]) break; // Ensures the virSerial is Sorted by breaking at the right time
					//	if (c == CircuitGUI::virSerial[v]) { compFound = 1; break; }
					//}
					//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));


					if (CircuitGUI::comp[c].bounds.intersects(CircuitGUI::selSqr.getGlobalBounds())) {
						if (!compFound)
						{
							CircuitGUI::virSerial.insert(CircuitGUI::virSerial.begin() + v, c);
							//CircuitGUI::virSprite.insert(CircuitGUI::virSprite.begin() + v, CircuitGUI::comp[c].sprite);
							//CircuitGUI::virSprite[v].setColor(tempDimColor);

							/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
							virSprite.back().setColor(tempDimColor);*/
						}
					}
					else if (compFound)
					{
						CircuitGUI::virSerial.erase(CircuitGUI::virSerial.begin() + v);
						//CircuitGUI::virSprite.erase(CircuitGUI::virSprite.begin() + v);
					}

				}
				//if (0 && ShiftPressed) {
				//	for (int vs = 0; vs < virSerialShift.size(); vs++) {
				//		static bool found = 0;
				//		for (int v = 0; v < virSerial.size(); v++) {
				//			if (virSerial[v] == virSerialShift[vs]) {
				//				found = 1;
				//				break;
				//			}
				//		}
				//		if (!found) {
				//			virSerial.emplace_back(virSerialShift[vs]);
				//			virSprite.emplace_back(comp[virSerial.back()].sprite);
				//		}
				//	}
				//}

				CircuitGUI::updateAllSqr();
			}
			else { static sf::Vector2f zero(0, 0); CircuitGUI::selSqr.setSize(zero); }

			/*Wire*/
			//asdfif (wireBool) { stimuliDisplay = 1; /*cout << "11";*/ wires.back().makeWire(); }
		}



		// ----------------------------------------	Update
		; {
			if (MInTool) { stimuliDisplay = 1; /*cout << "12";*/ }
			//circuit.updateWin();

			/*ImGui*/
			ImGui::SFML::Update(CircuitGUI::app, deltaClock.restart());
			/*
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
			testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));//*/

			//*
			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {
					stimuliDisplay = 1; /*cout << "a";*/
					if (ImGui::MenuItem("Open...", "Ctrl + O")) {
						stimuliDisplay = 1; /*cout << "14";*/ stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (ImGui::MenuItem("Save", "Ctrl + S")) {
						;
						//_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
						//_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)
					}
					if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {

						_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
						_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)

						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty()) {

							if (filepath.back() == 'T')
								CircuitGUI::Options::savef(filepath);

							if (filepath.back() == 'G')
								CircuitGUI::Options::printScreen(filepath);

						}

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) { CircuitGUI::app.close(); End = 1; continue; }

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Options")) {
					stimuliDisplay = 1; /*cout << "b";*/
					if (ImGui::BeginMenu("Handdrawn Icons"))
					{
						ImGui::Text("Coming Soon...");
						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::BeginMenu("Theme")) {

						if (ImGui::MenuItem("Dark")) {
							stimuliDisplay = 1; /*cout << "15";*/

							CircuitGUI::darkLightMode = false;

							updateThemeColors();
						}
						if (ImGui::MenuItem("Light")) {
							stimuliDisplay = 1; /*cout << "16";*/

							CircuitGUI::darkLightMode = true;

							updateThemeColors();
						}

						ImGui::EndMenu();
					}

					//asdf
					/*ImGui::Separator();
					if (ImGui::MenuItem("Game")) {
						stimuliDisplay = 1; stimuliEndNodes = 1; //cout << "17";
						PlayMode = !PlayMode;

						CircuitGUI::comp.clear();
						CircuitGUI::virSerial.clear();
						CircuitGUI::virSprite.clear();
						CircuitGUI::virSerialShift.clear();

						//asdfwires.clear();

						if (PlayMode) {
							CircuitGUI::Options::openf("Saved-Projects\\Maze.TXT");
							//ShellExecute(0, 0, L"https://www.youtube.com/watch?v=6cRctjPRv6M", 0, 0, SW_SHOW);
						}
						else { wireBool = 0; PlayRot = 0; }
					}*/

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help")) {
					stimuliDisplay = 1; //cout << "c";
					if (ImGui::BeginMenu("Controls"))
					{
						if (ImGui::BeginTable("table_context_menu_2", 2))
						{

							static const std::string opt[11][2] = { //HardCode
								{ "Drag View", "Press Scroll-Wheel + Drag" },
								{ "Selection Mode", "Click + Hold + Drag" },
								{ "Select All",		"Ctrl + A" },
								{ "Rotate",			"Ctrl + R" },
								{ "Delete",			"Del" },
								{ "Copy",			"Ctrl + C" },
								{ "Paste",			"Ctrl + V" },
								{ "Open",			"Ctrl + O" },
								{ "Save As",		"Ctrl + Shift + S" },
								{ "Escape",			"Esc" },
								{ "New Components",	"Hover Left" }
							};

							ImGui::TableSetupColumn("Option");
							ImGui::TableSetupColumn("Keys");

							ImGui::TableHeadersRow();
							ImGui::TableNextRow();

							for (int row = 0; row < 11; row++) //HardCode
							{
								ImGui::TableSetColumnIndex(0);
								ImGui::Text(opt[row][0].c_str());
								ImGui::SameLine();

								ImGui::TableSetColumnIndex(1);
								//ImGui::Text(opt[row][1].c_str());

								bool sameLine = false;
								std::string Text = opt[row][1] + " ", temp = "";
								for (int i = 0; i < Text.length(); i++) {

									if (('0' <= Text[i] && Text[i] <= '9') || ('a' <= Text[i] && Text[i] <= 'z') || ('A' <= Text[i] && Text[i] <= 'Z')) {
										temp += Text[i];
									}
									else if (Text[i] == '+') {
										if (sameLine) ImGui::SameLine();
										ImGui::Text("+");
										sameLine = true;
									}
									else {
										if (temp.size()) {
											if (sameLine) ImGui::SameLine();
											ImGui::SmallButton(temp.c_str());
											temp.clear();
											sameLine = true;
										}
									}

								}

								ImGui::TableNextRow();
							}
							ImGui::EndTable();
						}

						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::BeginMenu("Contacts"))
					{
						static std::string links[2][2] = // HardCode
						{
							{ "Copy Github", "https://github.com/IrfanJames/CircuitSimulator_SFML" },
							{ "Copy Email ", "irfanjamespak@gmail.com" }
						};

						for (int i = 0; i < 2; i++) {
							if (ImGui::Button(links[i][0].c_str()))
								sf::Clipboard::setString(links[i][1]);
							ImGui::SameLine();
							ImGui::Text(links[i][1].c_str());
						}

						ImGui::EndMenu();
					}

					/*ImGui::Separator();

					if (ImGui::BeginMenu("Options"))
					{
						static bool enabled = true;
						ImGui::MenuItem("Enabled", "", &enabled);
						ImGui::BeginChild("child", ImVec2(0, 60), true);
						for (int i = 0; i < 10; i++)
							ImGui::Text("Scrolling Text %d", i);
						ImGui::EndChild();
						static float f = 0.5f;
						static int n = 0;
						ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
						ImGui::InputFloat("Input", &f, 0.1f);
						ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
						ImGui::EndMenu();
					}*/

					ImGui::EndMenu();
				}

				/*ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
				{
					static ImGuiTabBarFlags tab_items_flags = ImGuiTabBarFlags_Reorderable;
					static bool a = 1, b = 1, c = 1;
					std::string fileNameIm = "Untitled-" + std::to_string(10) + ".txt";
					if (ImGui::BeginTabItem(fileNameIm.c_str(), &a, tab_items_flags))
					{
						//ImGui::Text("This is the Avocado tab!");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Broccoli", &b, tab_items_flags))
					{
						//ImGui::Text("This is the Broccoli tab!");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Cucumber", &c, tab_items_flags))
					{
						//ImGui::Text("This is the Cucumber tab!");
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}*/

				ImGui::EndMainMenuBar();
			}//*/

			/*if (ImGui::TreeNode("Advanced & Close Button"))
			{
				// Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
				static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
				ImGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", &tab_bar_flags, ImGuiTabBarFlags_Reorderable);
				ImGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", &tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
				ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
				ImGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", &tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
				if ((tab_bar_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
					tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
				if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
					tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
				if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
					tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

				// Tab Bar
				const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
				static bool opened[4] = { true, true, true, true }; // Persistent user state
				for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
				{
					if (n > 0) { ImGui::SameLine(); }
					ImGui::Checkbox(names[n], &opened[n]);
				}

				// Passing a bool* to BeginTabItem() is similar to passing one to Begin():
				// the underlying bool will be set to false when the tab is closed.
				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
				{
					for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
						if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
						{
							ImGui::Text("This is the %s tab!", names[n]);
							if (n & 1)
								ImGui::Text("I am an odd tab.");
							ImGui::EndTabItem();
						}
					ImGui::EndTabBar();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("TabItemButton & Leading/Trailing flags"))
			{
				static ImVector<int> active_tabs;
				static int next_tab_id = 0;
				if (next_tab_id == 0) // Initialize with some default tabs
					for (int i = 0; i < 3; i++)
						active_tabs.push_back(next_tab_id++);

				// TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
				// (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
				// but they tend to make more sense together)
				static bool show_leading_button = true;
				static bool show_trailing_button = true;
				ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
				ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

				// Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
				static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
				ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
				if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
					tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
				if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
					tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

				if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
				{
					// Demo a Leading TabItemButton(): click the "?" button to open a menu
					if (show_leading_button)
						if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
							ImGui::OpenPopup("MyHelpMenu");
					if (ImGui::BeginPopup("MyHelpMenu"))
					{
						ImGui::Selectable("Hello!");
						ImGui::EndPopup();
					}

					// Demo Trailing Tabs: click the "+" button to add a new tab (in your app you may want to use a font icon instead of the "+")
					// Note that we submit it before the regular tabs, but because of the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
					if (show_trailing_button)
						if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
							active_tabs.push_back(next_tab_id++); // Add new tab

					// Submit our regular tabs
					for (int n = 0; n < active_tabs.Size; )
					{
						bool open = true;
						char name[16];
						snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
						if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
						{
							ImGui::Text("This is the %s tab!", name);
							ImGui::EndTabItem();
						}

						if (!open)
							active_tabs.erase(active_tabs.Data + n);
						else
							n++;
					}

					ImGui::EndTabBar();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}*/

			/*ImGui::Dummy(ImVec2(0, 20));

			ImGui::BeginTabBar("#Additional Parameters");
			float value = 0.0f;
			if (ImGui::BeginTabItem("Tab Name2")) {
				ImGui::SliderFloat("Slider", &value, 0, 1.0f);
			}
			if (ImGui::BeginTabItem("Tab Name3")) {
				ImGui::Text("Tab 2");
			}
			if (ImGui::BeginTabItem("Tab Name4")) {
				ImGui::Text("Tab 3");
			}
			if (ImGui::BeginTabItem("Tab Name5")) {
				ImGui::Text("Tab 4");
			}
			ImGui::EndTabBar();*/

			//ImGui::Begin("Right-Click");
			//if (ImGui::BeginPopupContextItem()) {
			//	if (ImGui::Selectable("Apple")) cout<<"Apple";
			//	if (ImGui::Selectable("Banana")) cout << "Banana";
			//	ImGui::EndPopup();
			//}
			//ImGui::End;

			/*
			if (debugBool) {
				//if (ImGui::BeginMenu("Hello")) {
				if (ImGui::OpenPopupOnItemClick) {

					if (ImGui::Selectable("Apple")) cout << "Apple";
					if (ImGui::Selectable("Banana")) cout << "Banana";

					if (ImGui::MenuItem("One", "Ctrl + O")) { cout << "\nOpen"; }
					if (ImGui::MenuItem("Two", "Ctrl + S")) { cout << "\nSave"; }
					if (ImGui::MenuItem("Save as Image")) { cout << "\nOhh Yeah"; printScreen(); }
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) { cout << "\nExit"; }

					//ImGui::EndMenu();
					//ImGui::EndPopup();
				}
			}
			//*/


			if (Drag) {
				stimuliDisplay = 1; /*cout << "18"*/;
				CircuitGUI::Drag();
				CircuitGUI::colorBrightLineGrid();

				CircuitGUI::updatePosToolBox();

			}
			//testCircle.setPosition(CircuitGUI::onScreen(300, 300));

			//Tool Win
			if (!Drag) CircuitGUI::toolCol.setPosition((MInTool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MInTool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x - CircuitGUI::c_toolColWidth - t_TollWx) / 7), CircuitGUI::toolWinRestPos.y);
			if (Drag)  CircuitGUI::toolCol.setPosition(CircuitGUI::onScreen(0, 0));

			//Tool Sqr
			t_TollWx = CircuitGUI::ToolBoxLittleBox.getPosition().x;
			if (!Drag) CircuitGUI::ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MIntool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x - CircuitGUI::c_toolColWidth - t_TollWx) / 7), CircuitGUI::toolWinRestPos.y + CircuitGUI::trim(sf::Mouse::getPosition(CircuitGUI::app).y, CircuitGUI::c_toolColWidth));
			if (Drag)  CircuitGUI::ToolBoxLittleBox.setPosition(CircuitGUI::onScreen(0, 0 + CircuitGUI::trim(sf::Mouse::getPosition(CircuitGUI::app).y, CircuitGUI::c_toolColWidth)));
			if (MIntool) serialToolMouse = (int)(sf::Mouse::getPosition(CircuitGUI::app).y / CircuitGUI::c_toolColWidth); else serialToolMouse = 0;

			/*endNodes*/
			if (stimuliEndNodes) {
				CircuitGUI::updateAllEnds();
				CircuitGUI::updateEndCircles();
				CircuitGUI::updateAllSqr();


				//if (comp.size() < 1000)
				//{
				//	CircuitGUI::qtUpdate();
				//	CircuitGUI::qtWrite();
				//}
				//else if (comp.size() < 10000)
				//{
				//	CircuitGUI::qtUpdate();
				//	std::cout << "\nNot Printing QuadTree [No. of Elements(" << comp.size() << ") is more that 1000]\n\t* Not Printing because the sheer amount of time it would taken\n";
				//}
				//else {
				//	std::cout << "\nNot Printing QuadTree [No. of Elements(" << comp.size() << ") is more that 1000]\n\t* Not Printing because the sheer amount of time it would take";
				//	std::cout << "\nNot Making   QuadTree [No. of Elements(" << comp.size() << ") is more that 10000]\n\t* Not Making Quad Tree Because Program Not Fast Enough YET\n";
				//}
			}

			/*if (PlayRot) { CircuitGUI::view.rotate(0.9); }
			else CircuitGUI::view.setRotation(0);*/
		}


		// ----------------------------------------	Draw
		if (1 || stimuliDisplay) { // zero causes 100 cpu load
			CircuitGUI::app.setView(CircuitGUI::view);
			CircuitGUI::app.clear(CircuitGUI::backColor);

			drawGrid();

			if (virSerial.size() > 1) drawAllSqr();

			//qtDraw(CircuitGUI::qt);

			drawComp();

			/*Wires*/ /* {
				//for (int c = 0; c < wires.size(); c++) wires[c].draw(CircuitGUI::app);
			}*/

			drawNodes();

			/*//asdfif (!PlayMode)*/ drawBoarders();


			if (CircuitGUI::Occupied) drawVirSprites();

			drawSelSqr();

			drawToolColumn(MInTool, MIntool);

			/*ImGui*/ {
				//if (DrawCircle) CircuitGUI::app.draw(testCircle);
				ImGui::SFML::Render(CircuitGUI::app);//Last Thing to render
			}

		}
		CircuitGUI::app.display();


		CircuitGUI::app.setTitle("CircuitSim   " + std::to_string((float)(((float)clock() - (float)FrameTime_for_FPS) / (float)CLOCKS_PER_SEC) * 1000.0F) + " | " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)FrameTime_for_FPS))));
		FrameTime_for_FPS = clock();
		stimuliDisplay = 0; stimuliEndNodes = 0; CircuitGUI::Occupied = 0;
		/*cout << "\n";*/
	}
	
	
	/*ImGui*/
	ImGui::SFML::Shutdown();

#ifdef _DEBUG
	std::cin.get();
#else
	LOG.close();
#endif

	return 0;
}