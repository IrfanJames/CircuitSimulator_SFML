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

#include "Circuit_GUI.h"

//#include "clipboardxx.hpp"
#include <windows.h>

std::string OpenFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
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
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string SaveFileDialog(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
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

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

using std::cout;
//int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
int main(int argc, char* argv[]) {

	Entity::setFont("assets/Fonts/CalibriL_1.ttf");

	W = sf::VideoMode::getDesktopMode().width * 0.74;
	H = sf::VideoMode::getDesktopMode().height * 0.7;

	CircuitGUI::renderWinInit();
	CircuitGUI::viewInit();

	/*ImGui*/
	ImGui::SFML::Init(CircuitGUI::app);
	sf::Clock deltaClock;

	bool RELEASE_DEBUG = 1;
	bool End = 0, debugBool = 0;
	bool Drag = 0, stimuliDisplay = 1, stimuliEndNodes = 0;
	bool releaseBool = 1, wheelReleaseBool = 1, ShiftPressed=0;
	bool selectSquare = 0, mouseOnCompsBool = 0, wireBool = 0;
	int serialCompMouse = 0, serialToolMouse = 0, cursorInWin = 0;

	CircuitGUI::initializeGrid();
	CircuitGUI::loadTextures();

	
	/*ToolBox*/
	CircuitGUI::toolCol.setFillColor(sf::Color(0, 0, 0, 120));
	CircuitGUI::ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));
	CircuitGUI::toolSprInit();


	/*Vectors*/
	CircuitGUI::comp.reserve(3);
	CircuitGUI::allEnds.reserve(3);
	CircuitGUI::virSerial.reserve(3);
	CircuitGUI::virSprite.reserve(3);
	CircuitGUI::virSerialShift.reserve(3);
	//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);

	/*Wires*/
	std::vector<Wire> wires; wires.reserve(3);

	/*Sel Sqr*/
	sf::RectangleShape selSqr; {
		selSqr.setFillColor(sf::Color(66, 135, 245, 60));
		selSqr.setOutlineThickness(1.0f);
		selSqr.setOutlineColor(sf::Color(66, 135, 245)); }

	/*circleNode*/
	sf::CircleShape nodePic(4, 15); {
		nodePic.setOrigin(4, 4);
		nodePic.setFillColor(CircuitGUI::normalCompColor); }
	std::vector<sf::CircleShape> allEndCircles; allEndCircles.reserve(17);


	////////////////////////////////////////////// Solve
	//Graph circuit;
	//std::thread printScreenTread;

	///////////////////////////////////////////////
	
	//clipboardxx::clipboard clipboard;

	
	//*
	bool DrawCircle = 1;
	float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
	int t_vertices = 34;
	sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/


	if (1 < argc) {
		std::string temp(argv[1]);
		if (!temp.empty())
			CircuitOptions::openf(temp);
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

				//view = sf::View(sf::FloatRect(view.getCenter().x - ((float)evnt.size.width) / 2, view.getCenter().y - ((float)evnt.size.height) / 2, evnt.size.width, evnt.size.height));
				CircuitGUI::view.setCenter(CircuitGUI::view.getCenter()); CircuitGUI::view.setSize((float)evnt.size.width, (float)evnt.size.height);
				
				CircuitGUI::toolCol.setSize(sf::Vector2f(CircuitGUI::toolCol.getSize().x, CircuitGUI::view.getSize().y));
				CircuitGUI::updatePosToolBox();
			}

			if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left) {
				bool onNode = 0;
				static const int sensitivity = 7;
				sf::Vector2f cursorPos = CircuitGUI::cursorInSim();

				int e = 0;
				for (; e < CircuitGUI::allEnds.size(); e++) {
					if (abs(cursorPos.x - CircuitGUI::allEnds[e].x) < sensitivity && abs(cursorPos.y - CircuitGUI::allEnds[e].y) < sensitivity) { onNode = 1; break; }
				}

				if (onNode) {
					cout << ", On Node";

					if (!wireBool)wires.emplace_back(CircuitGUI::allEnds[e]);

					wireBool = !wireBool;
				}
				else {

					if (wireBool) wires.back().newEdge();

				}

			}
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left) { releaseBool = 1; }
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Middle) { wheelReleaseBool = 1; }

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == sf::Keyboard::Escape) { CircuitGUI::app.close(); End = 1; cout << "\n------------------ESC Pressed-----------------\n"; continue; }
				if (evnt.key.code == sf::Keyboard::Delete) {
					stimuliDisplay = 1;	stimuliEndNodes = 1;
					CircuitOptions::deletef();
				}
				if (evnt.key.code == sf::Keyboard::W) { wireBool = !wireBool; /*cout << "\ndebug\n";*/ }
				if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool; /*cout << "\ndebug\n";*/ }
				if (evnt.key.code == sf::Keyboard::P) {
					cout << "\nPrintScreen";

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
							CircuitOptions::printScreen(filepath);
					}
					//cout << "\n" << ((float)clock() - (float)print) / (float)CLOCKS_PER_SEC;
				}

				/*Ctrl*/
				if (evnt.key.control) {
					if (evnt.key.code == sf::Keyboard::A) {
						cout << "\nCtrl + A\n";

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

					}
					if (evnt.key.code == sf::Keyboard::R) {
						cout << "\nCtrl + R\n"; stimuliDisplay = 1;	stimuliEndNodes = 1;

						CircuitOptions::rotatef();
						
					}
					if (evnt.key.code == sf::Keyboard::O) {
						cout << "\nCtrl + O\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitOptions::openf(filepath);

					}
					if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {

						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty()) {

							if (filepath.back() == 'T')
								CircuitOptions::savef(filepath);

							if (filepath.back() == 'G')
								CircuitOptions::printScreen(filepath);

						}

					}
					if (evnt.key.code == sf::Keyboard::C) {
						cout << "\nCtrl + C\n";

						CircuitOptions::copyf();

					}
					if (evnt.key.code == sf::Keyboard::X) {
						cout << "\nCtrl + X\n"; stimuliDisplay = 1;	stimuliEndNodes = 1;

						// Copy
						{
							CircuitOptions::copyf();
						}

						// Delete
						{
							CircuitOptions::deletef();
						}
					}
					if (evnt.key.code == sf::Keyboard::V) {
						cout << "\nCtrl + V\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						CircuitOptions::pastef();

					}
				}
			}


			/*int difr = 10;
				if (evnt.key.code == Keyboard::Up) { view.setCenter(view.getCenter().x, view.getCenter().y - difr); }
				if (evnt.key.code == Keyboard::Down) { view.setCenter(view.getCenter().x, view.getCenter().y + difr); }
				if (evnt.key.code == Keyboard::Right) { view.setCenter(view.getCenter().x + difr, view.getCenter().y); }
				if (evnt.key.code == Keyboard::Left) { view.setCenter(view.getCenter().x - difr, view.getCenter().y); }*/
		}
		ShiftPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
		W = CircuitGUI::app.getSize().x; H = CircuitGUI::app.getSize().y;
		bool MInTool = !!(cursorInWin && 0 <= sf::Mouse::getPosition(CircuitGUI::app).x && sf::Mouse::getPosition(CircuitGUI::app).x <= CircuitGUI::c_toolColWidth);
		bool MIntool = !!(cursorInWin && MInTool && sf::Mouse::getPosition(CircuitGUI::app).y < (Entity::noOfComps - 1) * CircuitGUI::c_toolColWidth);
		float t_TollWx = CircuitGUI::toolCol.getPosition().x;
		//--------------------------------------------------------------------------------//











		// ----------------------------------------	Options
		{
			/*sf::Mouse Hold*/
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (releaseBool) {
					releaseBool = 0;
					CircuitGUI::click = clock();
					CircuitGUI::mouseHoldX = (float)sf::Mouse::getPosition(CircuitGUI::app).x; CircuitGUI::mouseHoldY = (float)sf::Mouse::getPosition(CircuitGUI::app).y;

					/*new Comp*/
					if (MIntool) {
						MIntool = 0;
						float tempNewCompX = 150 + CircuitGUI::view.getCenter().x - W / 2, tempNewCompY = 150 + CircuitGUI::view.getCenter().y - H / 2;
						tempNewCompX = CircuitGUI::trim(tempNewCompX, CircuitGUI::gap);
						tempNewCompY = CircuitGUI::trim(tempNewCompY, CircuitGUI::gap);

						CircuitGUI::comp.emplace_back(serialToolMouse, tempNewCompX, tempNewCompY, 0);
						CircuitGUI::comp.back().stimuli();

						/*Collisions*/
						tempNewCompX = CircuitGUI::trim(tempNewCompX, CircuitGUI::gap);
						while (CircuitGUI::occupiedAt(CircuitGUI::comp.size() - 1, sf::Vector2f(tempNewCompX, tempNewCompY))) {
							tempNewCompX = CircuitGUI::trim(tempNewCompX + 6 * CircuitGUI::gap, CircuitGUI::gap);

							if (tempNewCompX + 7 * CircuitGUI::gap - 150 - CircuitGUI::view.getCenter().x + W / 2 + 91 >= W) {
								tempNewCompX = 150 + CircuitGUI::view.getCenter().x - W / 2;
								tempNewCompY = CircuitGUI::trim(tempNewCompY + 6 * CircuitGUI::gap, CircuitGUI::gap);
							}
						}

						CircuitGUI::comp.back().x = tempNewCompX; CircuitGUI::comp.back().y = tempNewCompY;
						CircuitGUI::comp.back().stimuli();

						stimuliEndNodes = 1;

					}
					else {
						mouseOnCompsBool = 0;

						/*Check every component for Mouse*/
						for (int c = 0; !mouseOnCompsBool && c < CircuitGUI::comp.size(); c++) {

							if (CircuitGUI::comp[c].bounds.contains(CircuitGUI::cursorInSim())) {

								mouseOnCompsBool = 1; Drag = 0;

								if (!ShiftPressed) {
									CircuitGUI::virSerial.clear();
									CircuitGUI::virSprite.clear();
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

							}
						}

						//////////// Urgent need of enums , State Machine
						if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; selSqr.setPosition(CircuitGUI::cursorInSim()); }

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

				if (CircuitGUI::virSerial.size() > 0) mouseOnCompsBool = 1;

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
					if (CircuitGUI::Click(CircuitGUI::gap)) {
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
			if (CircuitGUI::mouseHoldX != (float)sf::Mouse::getPosition(CircuitGUI::app).x || CircuitGUI::mouseHoldY != (float)sf::Mouse::getPosition(CircuitGUI::app).y) {

				/*Follow Mouse*/
				if (mouseOnCompsBool) {
					if (!selectSquare /*&& releaseBool*/) {
						static int offSet[4][2] = {
							{0, -2},
							{2, 0},
							{0, 2},
							{-2, 0}
						};
						for (int c = 0; c < CircuitGUI::virSerial.size(); c++) {

							float tempX = (int)CircuitGUI::cursorInSim().x + CircuitGUI::gap * offSet[(int)CircuitGUI::comp[CircuitGUI::virSerial[c]].angle / 90][0]; ///
							float tempY = (int)CircuitGUI::cursorInSim().y + CircuitGUI::gap * offSet[(int)CircuitGUI::comp[CircuitGUI::virSerial[c]].angle / 90][1]; ///

							//float tempX = cursorInSim().x - mouseHoldX + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
							//float tempY = cursorInSim().y - mouseHoldY + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///

							//for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }
							for (int v = 0; v < CircuitGUI::virSprite.size(); v++) { CircuitGUI::virSprite[0].setPosition(tempX, tempY); }

							tempX = CircuitGUI::trim(tempX, CircuitGUI::gap);
							tempY = CircuitGUI::trim(tempY, CircuitGUI::gap);

							if (!CircuitGUI::occupiedAt(CircuitGUI::virSerial[c], sf::Vector2f(tempX, tempY))) {
								CircuitGUI::comp[CircuitGUI::virSerial[c]].x = (int)tempX; // += (were "=" before)
								CircuitGUI::comp[CircuitGUI::virSerial[c]].y = (int)tempY; // += (were "=" before)
								CircuitGUI::comp[CircuitGUI::virSerial[c]].stimuli();
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
				selSqr.setSize(sf::Vector2f(CircuitGUI::cursorInSim().x - selSqr.getPosition().x, CircuitGUI::cursorInSim().y - selSqr.getPosition().y));

				/*Selection*/
				for (int c = 0; c < CircuitGUI::comp.size(); c++) {

					bool compFound = 0;
					int v = 0;
					for (; v < CircuitGUI::virSerial.size();) {
						if (c == CircuitGUI::virSerial[v]) { compFound = 1; break; }
						else
							if (c < CircuitGUI::virSerial[v]) { break; }

						v++;
					}

					//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));

					if (CircuitGUI::comp[c].bounds.intersects(sf::FloatRect(selSqr.getPosition(), CircuitGUI::cursorInSim() - selSqr.getPosition()))) {
						if (!compFound) {
							//virSerial.emplace_back(c);
							CircuitGUI::virSerial.insert(CircuitGUI::virSerial.begin() + v, c);

							//virSprite.emplace_back(comp[virSerial.back()].sprite);
							CircuitGUI::virSprite.insert(CircuitGUI::virSprite.begin() + v, CircuitGUI::comp[c].sprite);
							/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
							virSprite.back().setColor(tempDimColor);*/
						}
					}
					else if (compFound) {
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

			}

			/*Wire*/
			if (wireBool) { stimuliDisplay = 1; wires.back().makeWire(); }
		}

		;
		// ----------------------------------------	Update
		{
			/*circuit.updateWin(); */

			if (MInTool) { stimuliDisplay = 1; }
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
			testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));//*/

			//*
			ImGui::SFML::Update(CircuitGUI::app, deltaClock.restart());
			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {

					if (ImGui::MenuItem("Open...", "Ctrl + O")) {
						cout << "\nOpen";

						cout << "\nCtrl + O\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							CircuitOptions::openf(filepath);

					}
					if (ImGui::MenuItem("Save", "Ctrl + S")) { cout << "\nSave"; }
					if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {
						cout << "\nShift Save";

						std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

						if (!filepath.empty()) {

							if (filepath.back() == 'T')
								CircuitOptions::savef(filepath);

							if (filepath.back() == 'G')
								CircuitOptions::printScreen(filepath);

						}

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) { CircuitGUI::app.close(); End = 1; cout << "\nEsc\n"; continue; }

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Options")) {
					
					if (ImGui::MenuItem("Handdrawn Icons")) { cout << "\nWobbly"; }
					if (ImGui::BeginMenu("Switch Themes")) {

						if (ImGui::MenuItem("Dark")) {
							cout << "\nDark"; stimuliDisplay = 1;

							CircuitGUI::gridColor.r = 100; CircuitGUI::gridColor.g = 105; CircuitGUI::gridColor.b = 110; CircuitGUI::gridColor.a = 20;
							CircuitGUI::backColor.r = 23;  CircuitGUI::backColor.g = 24;  CircuitGUI::backColor.b = 25;
							CircuitGUI::colorGrid();
						}
						if (ImGui::MenuItem("Light")) {
							cout << "\nLight"; stimuliDisplay = 1;

							CircuitGUI::gridColor.r = 212; CircuitGUI::gridColor.g = 232; CircuitGUI::gridColor.b = 247; CircuitGUI::gridColor.a = 50;
							CircuitGUI::backColor.r = 36;  CircuitGUI::backColor.g = 133; CircuitGUI::backColor.b = 202;
							CircuitGUI::colorGrid();
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help")) {

					if (ImGui::MenuItem("Controls")) { cout << "\nKeys"; }
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
			testCircle.setPosition(CircuitGUI::onScreen(0, 200));

			//Tool Win
			if (!Drag) CircuitGUI::toolCol.setPosition((MInTool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MInTool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x - CircuitGUI::c_toolColWidth - t_TollWx) / 7), CircuitGUI::toolWinRestPos.y);
			if (Drag)  CircuitGUI::toolCol.setPosition(CircuitGUI::onScreen(0, 0));

			//Tool Sqr
			t_TollWx = CircuitGUI::ToolBoxLittleBox.getPosition().x;
			if (!Drag) CircuitGUI::ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MIntool) * (t_TollWx + (CircuitGUI::toolWinRestPos.x - CircuitGUI::c_toolColWidth - t_TollWx) / 7), CircuitGUI::toolWinRestPos.y + CircuitGUI::trim(sf::Mouse::getPosition(CircuitGUI::app).y, CircuitGUI::c_toolColWidth));
			if (Drag)  CircuitGUI::ToolBoxLittleBox.setPosition(CircuitGUI::onScreen(0, 0 + CircuitGUI::trim(sf::Mouse::getPosition(CircuitGUI::app).y, CircuitGUI::c_toolColWidth)));
			if (MIntool) serialToolMouse = (int)(sf::Mouse::getPosition(CircuitGUI::app).y / CircuitGUI::c_toolColWidth); else serialToolMouse = 0;

			/*endNodes*/ {
				if (stimuliEndNodes) {

					CircuitGUI::updateAllEnds();

					while (CircuitGUI::allEnds.size() < allEndCircles.size()) {
						allEndCircles.pop_back();
					}

					while (allEndCircles.size() < CircuitGUI::allEnds.size()) {
						allEndCircles.emplace_back(nodePic);
					}

					for (int e = 0; e < allEndCircles.size(); e++) {
						allEndCircles[e].setPosition(CircuitGUI::allEnds[e]);
					}

					//cout << "\n" << allEnds.size();
				}
			}

		}


		// ----------------------------------------	Draw
		if (1 || stimuliDisplay) { // zero causes 100 cpu load
			CircuitGUI::app.setView(CircuitGUI::view);
			CircuitGUI::app.clear(CircuitGUI::backColor);

			/*grid*/ {
				for (int c = 0; c < CircuitGUI::vLines.size(); c++) { CircuitGUI::app.draw(CircuitGUI::vLines[c]); }
				for (int c = 0; c < CircuitGUI::hLines.size(); c++) { CircuitGUI::app.draw(CircuitGUI::hLines[c]); }
			}

			/*comp*/ {
				for (int c = 0; c < CircuitGUI::comp.size(); c++) { CircuitGUI::comp[c].draw(CircuitGUI::app); }
			}

			/*Wires*/ {
				for (int c = 0; c < wires.size(); c++) wires[c].draw(CircuitGUI::app);
			}

			/*Nodes*/ {
				for (int e = 0; e < allEndCircles.size(); e++) { CircuitGUI::app.draw(allEndCircles[e]); }
			}

			/*Boarders*/ {
				for (int v = 0; v < CircuitGUI::virSerial.size(); v++) { CircuitGUI::app.draw(CircuitGUI::comp[CircuitGUI::virSerial[v]].boarder); }
			}

			/*Virtual Sprites*/ {
				if (CircuitGUI::Occupied) for (int v = 0; v < CircuitGUI::virSprite.size(); v++) CircuitGUI::app.draw(CircuitGUI::virSprite[v]);
			}

			/*Selection Sqraure*/ {
				if (selectSquare && !releaseBool) CircuitGUI::app.draw(selSqr);
			}

			/*Tool Win*/ {
				if (MInTool) {
					CircuitGUI::app.draw(CircuitGUI::toolCol);
					
					for (int c = 0; c < (Entity::noOfComps - 1); c++) { CircuitGUI::ToolSpr[c].setColor(sf::Color(255, 255, 255, CircuitGUI::ToolSpr[c].getColor().a + (255 - CircuitGUI::ToolSpr[c].getColor().a) / 15)); }
					for (int c = 0; c < (Entity::noOfComps - 1); c++) { CircuitGUI::app.draw(CircuitGUI::ToolSpr[c]); }
				}
				else { for (int c = 0; c < (Entity::noOfComps - 1); c++) CircuitGUI::ToolSpr[c].setColor(sf::Color::Transparent); }

				if (MIntool) CircuitGUI::app.draw(CircuitGUI::ToolBoxLittleBox);
			}

			/*ImGui*/ {
				if (DrawCircle) CircuitGUI::app.draw(testCircle);
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