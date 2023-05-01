#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

#include <iostream>
#include <fstream>

//#include <vector>
#include <thread>
//#include <future>

/*ImGui*/
#include "imgui.h"
#include "imgui-SFML.h"

//#include "Circuit_Graph.hpp"
#include "SFML/Graphics.hpp"
#include "Circuit_Entity.hpp"
//asdf#include "Circuit_wire.hpp"
#include "Circuit_GUI.hpp"

//#include "clipboardxx.hpp"
#include <windows.h>

std::string OpenFileDialog(const char* filter, std::string name = "New");
std::string SaveFileDialog(const char* filter, std::string name = "New");
using std::cout;
using namespace CircuitGUI;
//int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {


int main(int argc, char* argv[]) {

//#ifdef _DEBUG
//	cout << "std::thread::hardware_concurrency(): " << std::thread::hardware_concurrency();
//#endif
	
	CircuitGUI::initializeGUI();
	for (int i = 1; i < argc; i++) {
		std::string temp(argv[i]);
		if (!temp.empty())
			CircuitGUI::Options::openf(argv[i]);
	}

	/*ImGui*/
	ImGui::SFML::Init(CircuitGUI::app);
	sf::Clock deltaClock;

	bool End = 0, debugBool = 0;
	bool Drag = 0, stimuliDisplay = 1, stimuliEndNodes = 0;
	bool releaseBool = 1, wheelReleaseBool = 1, ShiftPressed = 0;
	bool selectSquare = 0, Selection = 0, mouseOnCompsBool = 0, wireBool = 0;
	bool PlayMode = 0/*//asdf, PlayRot = 0*/;

	int serialCompMouse = 0, serialToolMouse = 0, cursorInWin = 0;

	//std::thread printScreenTread;
	//clipboardxx::clipboard clipboard;
	
	//* Test Circle
	bool DrawCircle = 1; int t_vertices = 34; float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 }; sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/
	

	/*Wires*/
	//asdfstd::vector<Wire> wires; wires.reserve(3);

	////////////////////////////////////////////// Solve

	//Graph circuit;

	///////////////////////////////////////////////



	time_t frame = clock();
	CircuitGUI::app.setKeyRepeatEnabled(false);
	sf::Event evnt;
	while (CircuitGUI::app.isOpen() && !End) {


		while (CircuitGUI::app.pollEvent(evnt)) {
			stimuliDisplay = 1; /*cout << "1";*/
			/*ImGui*/
			ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == evnt.Closed) { CircuitGUI::app.close(); End = 1; }
			if (evnt.type == evnt.MouseLeft) { cursorInWin = 0; }
			if (evnt.type == evnt.MouseEntered) { cursorInWin = 1; }
			if (evnt.type == evnt.Resized) {

				for (int s = 0; s < scenes.size(); s++) {
					scenes[s].view = sf::View(sf::FloatRect((int)scenes[s].view.getCenter().x - (int)(evnt.size.width / 2), (int)scenes[s].view.getCenter().y - (int)(evnt.size.height / 2), (int)evnt.size.width, (int)evnt.size.height));
				}
				//scenes[csi].view.setSize((int)evnt.size.width, (int)evnt.size.height);
				//scenes[csi].view.setCenter((int)scenes[csi].view.getCenter().x, (int)scenes[csi].view.getCenter().y);

				CircuitGUI::toolCol.setSize(sf::Vector2f(CircuitGUI::toolCol.getSize().x, scenes[csi].view.getSize().y));
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
					scenes[csi].virSprite.reserve(scenes[csi].virSerial.size());
					/*for (int v = 0; v < virSerial.size(); v++) {
						virSprite.emplace_back(comp[virSerial[v]].sprite);
						virSprite.back().setColor(CircuitGUI::tempDimColor); }*/


					for (int v = 0; v < scenes[csi].virSerial.size() && v < scenes[csi].virSprite.size(); v++) {
						scenes[csi].virSprite[v] = scenes[csi].comp[scenes[csi].virSerial[v]].sprite;
						scenes[csi].virSprite[v].setColor(CircuitGUI::tempDimColor);
					}

					if (scenes[csi].virSerial.size() < scenes[csi].virSprite.size())
					{
						scenes[csi].virSprite.erase(scenes[csi].virSprite.begin() + (int)scenes[csi].virSerial.size(), scenes[csi].virSprite.end());
					}

					if (scenes[csi].virSerial.size() > scenes[csi].virSprite.size())
					{
						for (int v = scenes[csi].virSprite.size(); v < scenes[csi].virSerial.size(); v++) {
							scenes[csi].virSprite.emplace_back(scenes[csi].comp[scenes[csi].virSerial[v]].sprite);
							scenes[csi].virSprite.back().setColor(CircuitGUI::tempDimColor);
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
				//if (evnt.key.code == sf::Keyboard::W) { wireBool = !wireBool; }
				//if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool;}
				if (0 /*&& evnt.key.code == sf::Keyboard::P*/) {

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
						stimuliDisplay = 1; /*cout << "3";*/

						while (scenes[csi].comp.size() < scenes[csi].virSerial.size()) {
							scenes[csi].virSerial.pop_back();
							scenes[csi].virSprite.pop_back();
						}

						for (int v = 0; v < scenes[csi].virSerial.size(); v++) {
							scenes[csi].virSerial[v] = v;
							scenes[csi].virSprite[v] = scenes[csi].comp[scenes[csi].virSerial[v]].sprite;
						}

						for (int c = scenes[csi].virSerial.size(); c < scenes[csi].comp.size(); c++) {
							scenes[csi].virSerial.emplace_back(c);
							scenes[csi].virSprite.emplace_back(scenes[csi].comp[scenes[csi].virSerial.back()].sprite);
						}

						CircuitGUI::updateAllSqr();
					}
					if (evnt.key.code == sf::Keyboard::R) {
						stimuliDisplay = 1;	/*cout << "4";*/ stimuliEndNodes = 1;

						CircuitGUI::Options::rotatef();

					}
					if (evnt.key.code == sf::Keyboard::O) {
						stimuliDisplay = 1; /*cout << "5";*/ stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (evnt.key.code == sf::Keyboard::N) {
						stimuliDisplay = 1; /*cout << "14";*/ stimuliEndNodes = 1;
						scenes.emplace_back();
						csi = scenes.size() - 1 * (!!scenes.size());
						CircuitGUI::initializeScene();
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
					if (evnt.key.code == sf::Keyboard::S && !evnt.key.shift) {
						std::string filepath = scenes[csi].path;
						std::ifstream file(filepath);

						if (file.fail())
							filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0", scenes[csi].getName());//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty())
							CircuitGUI::Options::savef(filepath);
					}
					if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {
						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0",scenes[csi].getName());//JPEG (*.JPG)\0*.JPG\0

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
			if (evnt.key.code == sf::Keyboard::Up)		{ scenes[csi].view.move(0, -difr); }
			if (evnt.key.code == sf::Keyboard::Down)	{ scenes[csi].view.move(0, difr); }
			if (evnt.key.code == sf::Keyboard::Right)	{ scenes[csi].view.move(difr, 0); }
			if (evnt.key.code == sf::Keyboard::Left)	{ scenes[csi].view.move(-difr, 0); }*/

		}
		ShiftPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
		W = CircuitGUI::app.getSize().x; H = CircuitGUI::app.getSize().y;
		bool MInTool = !!(cursorInWin && 0 <= sf::Mouse::getPosition(CircuitGUI::app).x && sf::Mouse::getPosition(CircuitGUI::app).x <= CircuitGUI::c_toolColWidth);
		bool MIntool = !!(cursorInWin && MInTool && sf::Mouse::getPosition(CircuitGUI::app).y < (Entity::noOfComps - 1)* CircuitGUI::c_toolColWidth);
		float t_TollWx = CircuitGUI::toolCol.getPosition().x;
		//--------------------------------------------------------------------------------//






		/*if (PlayRot) { scenes[csi].view.rotate(0.9); }
		else scenes[csi].view.setRotation(0);*/


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
						float tempNewCompX = 150 + scenes[csi].view.getCenter().x - W / 2, tempNewCompY = 150 + scenes[csi].view.getCenter().y - H / 2;
						tempNewCompX = CircuitGUI::trim(tempNewCompX);
						tempNewCompY = CircuitGUI::trim(tempNewCompY);

						scenes[csi].comp.emplace_back(serialToolMouse, tempNewCompX, tempNewCompY, 0);
						scenes[csi].comp.back().stimuli();

						/*Collisions*/
						tempNewCompX = CircuitGUI::trim(tempNewCompX, CircuitGUI::gap);
						while (CircuitGUI::occupiedAt(scenes[csi].comp.size() - 1, sf::Vector2f(tempNewCompX, tempNewCompY))) {
							tempNewCompX = CircuitGUI::trim(tempNewCompX + 6 * CircuitGUI::gap);

							if (tempNewCompX + 7 * CircuitGUI::gap - 150 - scenes[csi].view.getCenter().x + W / 2 + 91 >= W) {
								tempNewCompX = 150 + scenes[csi].view.getCenter().x - W / 2;
								tempNewCompY = CircuitGUI::trim(tempNewCompY + 6 * CircuitGUI::gap);
							}
						}

						scenes[csi].comp.back().x = (int)trim(tempNewCompX); scenes[csi].comp.back().y = (int)trim(tempNewCompY);
						scenes[csi].comp.back().stimuli();

						stimuliEndNodes = 1;

					}
					else {
						mouseOnCompsBool = 0;

						if (scenes[csi].virSerial.size() > 0 && scenes[csi].allSqr.getGlobalBounds().contains(CircuitGUI::cursorInSim())) mouseOnCompsBool = 1;
						else {

							/*Check every component for Mouse*/
							for (int c = 0; /*!mouseOnCompsBool &&*/ c < scenes[csi].comp.size(); c++) {

								if (scenes[csi].comp[c].bounds.contains(CircuitGUI::cursorInSim())) {

									mouseOnCompsBool = 1; Drag = 0;

									if (!ShiftPressed) {
										scenes[csi].virSerial.clear();
										scenes[csi].virSprite.clear();
										scenes[csi].virSerialShift.clear();
									}
									scenes[csi].virSerial.emplace_back(c);

									//copied in sel square selection
									scenes[csi].virSprite.emplace_back(scenes[csi].comp[scenes[csi].virSerial.back()].sprite);
									scenes[csi].virSprite.back().setOrigin(scenes[csi].virSprite.back().getTexture()->getSize().x / 2, scenes[csi].virSprite.back().getTexture()->getSize().y / 2);
									scenes[csi].virSprite.back().setColor(CircuitGUI::tempDimColor);/*
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
					scenes[csi].virSerialShift = scenes[csi].virSerial;


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
					if (/*//asdf!PlayMode &&*/ CircuitGUI::Click(CircuitGUI::gap)) {
						stimuliDisplay = 1; /*cout << "8";*/

						for (int v = 0; v < scenes[csi].virSerial.size(); v++) {

							if (scenes[csi].comp[scenes[csi].virSerial[v]].bounds.contains(CircuitGUI::cursorInSim())) {

								if (scenes[csi].comp[scenes[csi].virSerial[v]].serial == 5)scenes[csi].comp[scenes[csi].virSerial[v]].serial = 7;
								else if (scenes[csi].comp[scenes[csi].virSerial[v]].serial == 7) scenes[csi].comp[scenes[csi].virSerial[v]].serial = 5;

								scenes[csi].comp[scenes[csi].virSerial[v]].sprite.setTexture(compTex[scenes[csi].comp[scenes[csi].virSerial[v]].serial]);

								scenes[csi].virSerial.erase(scenes[csi].virSerial.begin() + v);
								scenes[csi].virSprite.erase(scenes[csi].virSprite.begin() + v);

								break;
							}

						}

					}

					if (CircuitGUI::Click(3)) {
						scenes[csi].virSprite.clear();  ///// 
						scenes[csi].virSerial.clear();  ///// 
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
					CircuitGUI::initializeDrag();
				}
				wheelReleaseBool = 0;
			}
			else { Drag = 0; }

			/*Continoue while hold*/
			if (!selectSquare && mouseOnCompsBool /*//asdf&& !PlayMode*/ && !Click(0) /*&& releaseBool*/) {

				CircuitGUI::Responses::followCoursor();

				stimuliEndNodes = 1; stimuliDisplay = 1; /*cout << "9";*/
			}

			/*Select Sqr*/
			if (selectSquare && !releaseBool /*//asdf&& !PlayMode*/) {

				CircuitGUI::Responses::selection();

				stimuliDisplay = 1; /*cout << "10";*/
				Selection = 1;
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

			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {
					stimuliDisplay = 1; /*cout << "a";*/
					if (ImGui::MenuItem("New", "Ctrl + N")) {
						stimuliDisplay = 1; /*cout << "14";*/ stimuliEndNodes = 1;
						scenes.emplace_back();
						csi = scenes.size() - 1 * (!!scenes.size());
						CircuitGUI::initializeScene();
					}
					if (ImGui::MenuItem("Open...", "Ctrl + O")) {
						stimuliDisplay = 1; /*cout << "14";*/ stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0", scenes[csi].getName());

						if (!filepath.empty())
							CircuitGUI::Options::openf(filepath);

					}
					if (ImGui::MenuItem("Save", "Ctrl + S")) {

						std::string filepath = scenes[csi].path;
						std::ifstream file(filepath);

						if (file.fail())
							filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0", scenes[csi].getName());//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty())
							CircuitGUI::Options::savef(filepath);
					}
					if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {

						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0", scenes[csi].getName());//JPEG (*.JPG)\0*.JPG\0

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

						scenes[csi].comp.clear();
						scenes[csi].virSerial.clear();
						scenes[csi].virSprite.clear();
						scenes[csi].virSerialShift.clear();

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
								{ "Drag View", "Press_ScrollWheel + Drag" },
								{ "Selection Mode", "Click + Hold + Drag" },
								{ "Select All",		"Ctrl + A" },
								{ "Rotate",			"Ctrl + R" },
								{ "Delete",			"Del" },
								{ "Copy",			"Ctrl + C" },
								{ "Paste",			"Ctrl + V" },
								{ "Open",			"Ctrl + O" },
								{ "Save As",		"Ctrl + Shift + S" },
								{ "Escape",			"Esc" },
								{ "New Components",	"Hover_Cursor_to_Left" }
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
									else if (Text[i] == '_') {
										temp += ' ';
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

				if (ImGui::BeginTabBar("#MyTabBar"))
				{
					//static int noOfScenes = 0;
					static ImGuiTabBarFlags tab_items_flags = ImGuiTabBarFlags_Reorderable;
					for (int s = 0; s < scenes.size(); s++) {
						if (ImGui::BeginTabItem(scenes[s].getName().c_str(), &scenes[s].isOpen, tab_items_flags))
						{
							CircuitGUI::csi = s;
							CircuitGUI::updatePosToolBox();
							ImGui::EndTabItem();
						}
					}
					//csi = noOfScenes < scenes.size() ? scenes.size() - (!!scenes.size()) : csi;
					//noOfScenes = scenes.size();

					ImGui::EndTabBar();
				}
				ImGui::EndMainMenuBar();
			}

			/*Scenes*/ {
				for (int s = 0; s < scenes.size(); s++) {
					if (!scenes[s].isOpen) {
						scenes.erase(scenes.begin() + s);
						s--;
					}
				}
				if (scenes.size() == 0) {
					scenes.emplace_back();
					CircuitGUI::initializeScene();
				}
				csi = csi < scenes.size() ? csi : scenes.size() - 1 * (!!scenes.size());
			}
			
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
			/*
			if (ImGui::TreeNode("Advanced & Close Button"))
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
			}//*/
			/*
			ImGui::Begin("Right-Click");
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Selectable("Apple")) cout<<"Apple";
				if (ImGui::Selectable("Banana")) cout << "Banana";
				ImGui::EndPopup();
			}
			ImGui::End;//*/
			

			if (Drag) {
				stimuliDisplay = 1; /*cout << "18"*/;
				CircuitGUI::Drag();
				CircuitGUI::colorBrightLineGrid();

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

				//CircuitGUI::qtUpdate();
				//CircuitGUI::qtWrite();
			}

		}



		// ----------------------------------------	Draw
		if (1 || stimuliDisplay) { // zero causes 100 cpu load
				CircuitGUI::app.setView(scenes[csi].view);
				CircuitGUI::app.clear(CircuitGUI::backColor);

				drawGrid();

				if (scenes[csi].virSerial.size() > 1) drawAllSqr();

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


		CircuitGUI::app.setTitle("CircuitSim   " + std::to_string((float)(((float)clock() - (float)frame) / (float)CLOCKS_PER_SEC) * 1000.0F) + " | " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
		frame = clock();
		stimuliDisplay = 0; stimuliEndNodes = 0; CircuitGUI::Occupied = 0;
		/*cout << "\n";*/
	}



	stimuliDisplay = 1; /*cout << "19";*/ stimuliEndNodes = 1;

	/*ImGui*/
	ImGui::SFML::Shutdown();
	std::cin.get();
	return 0;
}

std::string OpenFileDialog(const char* filter, std::string name)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	for (int c = 0; c < name.size(); c++)
		szFile[c] = (int)name[c];
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

std::string SaveFileDialog(const char* filter, std::string name)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	for (int c = 0; c < name.size(); c++)
		szFile[c] = (int)name[c];
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

/*
#include <direct.h>

cout << _mkdir("Awesomness");
	system("pause");
	cout << _rmdir("Awesomness");*/

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

/*sf::Cursor cursor;
	if (MIntool) {
		cursor.loadFromSystem(sf::Cursor::Hand);
		CircuitGUI::app.setMouseCursor(cursor);
	} else {
		cursor.loadFromSystem(sf::Cursor::Arrow);
		CircuitGUI::app.setMouseCursor(cursor);
	}*/