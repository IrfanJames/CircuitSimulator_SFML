#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

#include <iostream>
#include <fstream>
//#include <thread>
//#include <future>
//#include <vector>

/*ImGui*/
#include "imgui.h"
#include "imgui-SFML.h"

//#include "Circuit_Graph.h"
#include "SFML/Graphics.hpp"
#include "Circuit_Entity.h"
#include "Circuit_wire.h"
#include "Circuit_GUI.hpp"


//#include "clipboardxx.hpp"
#include <windows.h>

std::string OpenFileDialog(const char* filter, int tempNumber = 0);
std::string SaveFileDialog(const char* filter, int tempNumber = 1000);
using std::cout;
using namespace CircuitGUI;
//int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
int main(int argc, char* argv[]) {
	CircuitGUI::initializeGUI();

	/*ImGui*/
	ImGui::SFML::Init(CircuitGUI::app);
	sf::Clock deltaClock;

	bool RELEASE_DEBUG = 1;
	bool End = 0, debugBool = 0;
	bool Drag = 0, stimuliDisplay = 1, stimuliEndNodes = 0;
	bool releaseBool = 1, wheelReleaseBool = 1, ShiftPressed=0;
	bool selectSquare = 0, mouseOnCompsBool = 0, wireBool = 0;
	bool PlayMode = 0, PlayRot = 0;

	int serialCompMouse = 0, serialToolMouse = 0, cursorInWin = 0;

	//std::thread printScreenTread;
	//clipboardxx::clipboard clipboard;
	//* //Test Circle
	bool DrawCircle = 1; int t_vertices = 34; float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 }; sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/
	

	/*Wires*/
	std::vector<Wire> wires; wires.reserve(3);


	////////////////////////////////////////////// Solve
	
	//Graph circuit;

	///////////////////////////////////////////////

	if (1 < argc) {
		std::string temp(argv[1]);
		if (!temp.empty())
			CircuitGUI::Options::openf(temp);
	}

	time_t frame = clock();
	/*Ravi*/ CircuitGUI::app.setKeyRepeatEnabled(false);
	while (CircuitGUI::app.isOpen() && !End) {

		sf::Event evnt;
		while (CircuitGUI::app.pollEvent(evnt)) {
			stimuliDisplay = 1;
			/*ImGui*/
			ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == evnt.Closed) { CircuitGUI::app.close(); End = 1; }
			if (evnt.type == evnt.MouseLeft) { cursorInWin = 0; }
			if (evnt.type == evnt.MouseEntered) { cursorInWin = 1; }
			if (evnt.type == evnt.Resized) {

				view = sf::View(sf::FloatRect((int)view.getCenter().x - (int)(evnt.size.width / 2), (int)view.getCenter().y - (int)(evnt.size.height/ 2), (int)evnt.size.width, (int)evnt.size.height));
				//CircuitGUI::view.setSize((int)evnt.size.width, (int)evnt.size.height);
				//CircuitGUI::view.setCenter((int)CircuitGUI::view.getCenter().x, (int)CircuitGUI::view.getCenter().y);

				CircuitGUI::toolCol.setSize(sf::Vector2f(CircuitGUI::toolCol.getSize().x, CircuitGUI::view.getSize().y));
				CircuitGUI::updatePosToolBox();
			}

			if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left) {
				bool onNode = 0;
				static int sensitivity = 7;
				sf::Vector2f cursorPos = CircuitGUI::cursorInSim();

				sf::Vector2f start(165, 180), stop(1245, 660);

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

			}
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left) { releaseBool = 1; }
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Middle) { wheelReleaseBool = 1; }

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == sf::Keyboard::Escape) { CircuitGUI::app.close(); End = 1; continue; }
				if (evnt.key.code == sf::Keyboard::Delete) {
					stimuliDisplay = 1;	stimuliEndNodes = 1;
					CircuitGUI::Options::deletef();
				}
				if (evnt.key.code == sf::Keyboard::W) { wireBool = !wireBool;}
				//if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool;}
				if (evnt.key.code == sf::Keyboard::P) {

					//time_t print = clock();

					/*sf::Texture tex;

					tex.update(app);

					sf::Image screenshot(tex.copyToImage());

					screenshot.saveToFile("screenshot.png");*/

					std::string filepath = SaveFileDialog("PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

					//screenshot = app.capture();
					//screenshot.saveToFile("screenshot.png");

					for (int c = 0; c < 1; c++) {
						//std::thread printScreenTread { printScreen }; printScreenTread.join();

						//std::async(std::launch::async, printScreen);
						if (!filepath.empty())
							CircuitGUI::Options::printScreen(filepath);
					}
					//cout << "\n" << ((float)clock() - (float)print) / (float)CLOCKS_PER_SEC;
				}
				//if (evnt.key.code == sf::Keyboard::G) { PlayMode = !PlayMode; }

				/*Ctrl*/
				if (evnt.key.control) {
					if (evnt.key.code == sf::Keyboard::A) {

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
						stimuliDisplay = 1;	stimuliEndNodes = 1;

						CircuitGUI::Options::rotatef();

					}
					if (evnt.key.code == sf::Keyboard::O) {
						stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {

						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty()) {

							if (filepath.back() == 'T')
								CircuitGUI::Options::savef(filepath);

							if (filepath.back() == 'G')
								CircuitGUI::Options::printScreen(filepath);

						}

					}
					if (evnt.key.code == sf::Keyboard::C) {

						CircuitGUI::Options::copyf();

					}
					if (evnt.key.code == sf::Keyboard::X) {
						stimuliDisplay = 1;	stimuliEndNodes = 1;

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
						stimuliDisplay = 1; stimuliEndNodes = 1;

						CircuitGUI::Options::pastef();

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
		W = CircuitGUI::app.getSize().x; H = CircuitGUI::app.getSize().y;
		bool MInTool = !!(cursorInWin && 0 <= sf::Mouse::getPosition(CircuitGUI::app).x && sf::Mouse::getPosition(CircuitGUI::app).x <= CircuitGUI::c_toolColWidth);
		bool MIntool = !!(cursorInWin && MInTool && sf::Mouse::getPosition(CircuitGUI::app).y < (Entity::noOfComps - 1) * CircuitGUI::c_toolColWidth);
		float t_TollWx = CircuitGUI::toolCol.getPosition().x;
		//--------------------------------------------------------------------------------//



		
		if (PlayRot) { CircuitGUI::view.rotate(0.9); }
		else CircuitGUI::view.setRotation(0);





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
					if (!PlayMode && CircuitGUI::Click(CircuitGUI::gap)) {
						stimuliDisplay = 1;

						for (int v = 0; v < CircuitGUI::virSerial.size(); v++) {

							if (CircuitGUI::comp[CircuitGUI::virSerial[v]].bounds.contains(CircuitGUI::cursorInSim())) {

								if (CircuitGUI::comp[CircuitGUI::virSerial[v]].serial == 5)CircuitGUI::comp[CircuitGUI::virSerial[v]].serial = 7;
								else if (CircuitGUI::comp[CircuitGUI::virSerial[v]].serial == 7) CircuitGUI::comp[CircuitGUI::virSerial[v]].serial = 5;

								CircuitGUI::comp[CircuitGUI::virSerial[v]].sprite.setTexture(compTex[CircuitGUI::comp[CircuitGUI::virSerial[v]].serial]);

								CircuitGUI::virSerial.erase(CircuitGUI::virSerial.begin() + v);
								CircuitGUI::virSprite.erase(CircuitGUI::virSprite.begin() + v);

								break;
							}

						}

					}

					if (CircuitGUI::Click(3)) {
						CircuitGUI::virSprite.clear();  ///// 
						CircuitGUI::virSerial.clear();  ///// 
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
				if (wheelReleaseBool /*!mouseOnCompsBool && !selectSquare*/) {
					Drag = 1;
					mouseOnCompsBool = 0;
					CircuitGUI::iniDrag();
				}
				wheelReleaseBool = 0;
			}
			else { Drag = 0; }

			/*Continoue while hold*/
			if (!selectSquare && mouseOnCompsBool && !PlayMode && !Click(0) /*&& releaseBool*/) {
				sf::FloatRect virArea = allSqr.getGlobalBounds();
				sf::Vector2f offsetPos((int)CircuitGUI::cursorInSim().x - (virArea.left + (int)(virArea.width / 2)), (int)CircuitGUI::cursorInSim().y - (virArea.top + (int)(virArea.height / 2)));

				/*static const sf::Vector2f offsetHold[4] = {
					{0, -2},
					{2, 0},
					{0, 2},
					{-2, 0}
				};*/

				//static sf::Vector2f offsetPos;
				//offsetPos.x = (int)CircuitGUI::cursorInSim().x - (int)CircuitGUI::comp[CircuitGUI::virSerial.front()].x; ///
				//offsetPos.y = (int)CircuitGUI::cursorInSim().y - (int)CircuitGUI::comp[CircuitGUI::virSerial.front()].y; ///
				//float tempX = cursorInSim().x - mouseHoldX + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
				//float tempY = cursorInSim().y - mouseHoldY + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///

				//CircuitGUI::gap * offSet[(int)CircuitGUI::comp[CircuitGUI::virSerial.front()].angle / 90][0];
				//CircuitGUI::gap * offSet[(int)CircuitGUI::comp[CircuitGUI::virSerial.front()].angle / 90][1];
				for (int v = 0; v < CircuitGUI::virSprite.size(); v++)
					CircuitGUI::virSprite[v].setPosition(CircuitGUI::cursorInSim() - sf::Vector2f(comp[0].x - comp[virSerial[v]].x, comp[0].y - comp[virSerial[v]].y));

				//for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }

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

				stimuliEndNodes = 1; stimuliDisplay = 1;
			}

			/*Select Sqr*/
			if (selectSquare && !releaseBool && !PlayMode) {
				stimuliDisplay = 1;

				/*Sel Sqr*/
				CircuitGUI::selSqr.setSize(CircuitGUI::cursorInSim() - CircuitGUI::selSqr.getPosition());

				/*Selection*/
				for (int c = 0; c < CircuitGUI::comp.size(); c++) {
					//cout << std::count(virSerial.begin(), virSerial.end(), c);
					//bool bi = std::binary_search(virSerial.begin(), virSerial.end(), c);
					//auto mk = std::find(virSerial.begin(), virSerial.end(), c);

					bool compFound = 0;
					int v = 0;
					for (; v < CircuitGUI::virSerial.size() && c >= CircuitGUI::virSerial[v]; v++) { // if(CircuitGUI::virSerial[v]) break; // Ensures the virSerial is Sorted by breaking at the right time
						if (c == CircuitGUI::virSerial[v]) { compFound = 1; break; }
					}
					//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));
					
					if (CircuitGUI::comp[c].bounds.intersects(CircuitGUI::selSqr.getGlobalBounds())) {
						if (!compFound)
						{
							CircuitGUI::virSerial.insert(CircuitGUI::virSerial.begin() + v, c);
							CircuitGUI::virSprite.insert(CircuitGUI::virSprite.begin() + v, CircuitGUI::comp[c].sprite);
							
							/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
							virSprite.back().setColor(tempDimColor);*/
						}
					}
					else if (compFound)
					{
						CircuitGUI::virSerial.erase(CircuitGUI::virSerial.begin() + v);
						CircuitGUI::virSprite.erase(CircuitGUI::virSprite.begin() + v);
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
			if (wireBool) { stimuliDisplay = 1; wires.back().makeWire(); }
		}


		// ----------------------------------------	Update
		;{
			if (MInTool) { stimuliDisplay = 1; }
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

					if (ImGui::MenuItem("Open...", "Ctrl + O")) {
						stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (ImGui::MenuItem("Save", "Ctrl + S")) { ; }
					if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {

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

					if (ImGui::MenuItem("Handdrawn Icons")) { /*cout << "\nWobbly";*/ }
					if (ImGui::BeginMenu("Switch Themes")) {

						if (ImGui::MenuItem("Dark")) {
							stimuliDisplay = 1;

							CircuitGUI::gridColor.r = 100; CircuitGUI::gridColor.g = 105; CircuitGUI::gridColor.b = 110; CircuitGUI::gridColor.a = 20;
							CircuitGUI::backColor.r = 23;  CircuitGUI::backColor.g = 24;  CircuitGUI::backColor.b = 25;
							CircuitGUI::colorGrid();
						}
						if (ImGui::MenuItem("Light")) {
							stimuliDisplay = 1;

							CircuitGUI::gridColor.r = 212; CircuitGUI::gridColor.g = 232; CircuitGUI::gridColor.b = 247; CircuitGUI::gridColor.a = 50;
							CircuitGUI::backColor.r = 36;  CircuitGUI::backColor.g = 133; CircuitGUI::backColor.b = 202;
							CircuitGUI::colorGrid();
						}

						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Game")) {
						PlayMode = !PlayMode;

						CircuitGUI::comp.clear();
						CircuitGUI::virSerial.clear();
						CircuitGUI::virSprite.clear();
						CircuitGUI::virSerialShift.clear();

						wires.clear();

						if (PlayMode) {
							CircuitGUI::Options::openf("Saved-Projects\\Maze.TXT");
							ShellExecute(0, 0, L"https://www.youtube.com/watch?v=6cRctjPRv6M", 0, 0, SW_SHOW);
						}
						else { wireBool = 0; PlayRot = 0; }

						stimuliDisplay = 1; stimuliEndNodes = 1;
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help")) {

					if (ImGui::MenuItem("Controls")) { /*cout << "\nKeys";*/ }
					ImGui::Separator();

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
					}


					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}//*/

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
				stimuliDisplay = 1;
				CircuitGUI::Drag();
				CircuitGUI::colorGrid();

				CircuitGUI::updatePosToolBox();
				
			}
			testCircle.setPosition(CircuitGUI::onScreen(300, 300));

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
			}

		}


		// ----------------------------------------	Draw
		if (1 || stimuliDisplay) { // zero causes 100 cpu load
			CircuitGUI::app.setView(CircuitGUI::view);
			CircuitGUI::app.clear(CircuitGUI::backColor);

			drawGrid();

			if (virSerial.size() > 1) drawAllSqr();

			drawComp();

			/*Wires*/ {
				for (int c = 0; c < wires.size(); c++) wires[c].draw(CircuitGUI::app);
			}

			drawNodes();

			if (!PlayMode) drawBoarders();

			if (CircuitGUI::Occupied) drawVirSprites();
			
			drawSelSqr();
			
			drawToolColumn(MInTool, MIntool);

			/*ImGui*/ {
				//if (DrawCircle) CircuitGUI::app.draw(testCircle);
				ImGui::SFML::Render(CircuitGUI::app);//Last Thing to render
			}

			CircuitGUI::app.display();
		}


		CircuitGUI::app.setTitle("CircuitSim   " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
		frame = clock();
		stimuliDisplay = 0; stimuliEndNodes = 0; CircuitGUI::Occupied = 0;
	}
	


	stimuliDisplay = 1; stimuliEndNodes = 1;
	
	/*ImGui*/
	ImGui::SFML::Shutdown();
	std::cin.get();
	return 0;
}

std::string OpenFileDialog(const char* filter, int tempNumber)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Project-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];	
	//---------------


	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	//ofn.hwndOwner = CircuitGUI::app.getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string SaveFileDialog(const char* filter, int tempNumber)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Untitled-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];
	//---------------

	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE) {
		/*LPCWSTR temp = (wchar_t*)ofn.lpstrFile;
		ShellExecute(NULL, NULL, temp, NULL, NULL, SW_SHOWNORMAL);*/
		return ofn.lpstrFile;
	}

	return std::string();
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

//if (RELEASE_DEBUG/* && stimuliEndNodes*/) {
//	while (virSerial.size() < allBoarders.size()) {
//		allBoarders.pop_back();
//	}
//	while (allBoarders.size() < virSerial.size()) {
//		allBoarders.emplace_back(boarderPic);
//	}
//	sf::FloatRect bounds;
//	for (int v = 0; v < virSerial.size(); v++) {
//		bounds = comp[virSerial[v]].getBounds();
//		allBoarders[v].setPosition(bounds.left, bounds.top);
//		allBoarders[v].setSize(sf::Vector2f(bounds.width, bounds.height));
//	}
//}

/*if (debugBool) {
			debugBool = 0;

			//LPCWSTR pszPathToOpen = L"C:\\Windows";
			//PIDLIST_ABSOLUTE pidl;
			//if (SUCCEEDED(SHParseDisplayName(pszPathToOpen, 0, &pidl, 0, 0)))
			//{
			//	// we don't want to actually select anything in the folder, so we pass an empty
			//	// PIDL in the array. if you want to select one or more items in the opened
			//	// folder you'd need to build the PIDL array appropriately
			//	ITEMIDLIST idNull = { 0 };
			//	LPCITEMIDLIST pidlNull[1] = { &idNull };
			//	SHOpenFolderAndSelectItems(pidl, 1, pidlNull, 0);
			//	ILFree(pidl);
			//}

			ShellExecute(NULL, NULL, L"E:\\Programming\\C++\\CircuitSimulator_SFML\\Circiut_Sim\\Saved-Images", NULL, NULL, SW_SHOWNORMAL);

			//system("start explorer c:\\php");

		}*/