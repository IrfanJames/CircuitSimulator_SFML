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

#include "SFML/Graphics.hpp"

#include "Circuit_Entity.h"
#include "Circuit_wire.h"
//#include "Circuit_Graph.h"


#include <windows.h>



//#include "clipboardxx.hpp"

using namespace sf;
using std::cout;

sf::Vector2f cursorInSim(const sf::RenderWindow& App);
bool Click(const sf::RenderWindow& App, int Sensitivity);
float trim(float num, int wrt);

bool occupiedAt(int Index, const sf::Vector2f& At);

void updateAllEnds();
void printScreen(const std::string& filepath);
void savef(const std::string& file);
void openf(int Gap, const std::string& filepath, std::vector<int>& vir, std::vector<int>& virShft, std::vector<sf::Sprite>& virSpr);
void copyf(const std::vector<int>& vir);
void pastef(int Gap);
void rotatef(const std::vector<int>& vir);
void deletef(std::vector<int>& vir, std::vector<int>& virShft, std::vector<sf::Sprite>& virSpr);

bool Occupied = 0;
float mouseHoldX, mouseHoldY;
time_t click = clock(); // Time passed since Click

std::vector<Entity> comp;
std::vector<sf::Vector2f> allEnds;



std::string OpenFile(const char* filter)
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

std::string SaveFile(const char* filter)
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


//int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
int main() {




	Entity::setFont("assets/Fonts/CalibriL_1.ttf");

	W = sf::VideoMode::getDesktopMode().width * 0.74;
	H = sf::VideoMode::getDesktopMode().height * 0.7;

//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
	sf::ContextSettings settings; settings.antialiasingLevel = 8;
	sf::RenderWindow app/*(;
	app.create*/(VideoMode(W, H), "CircuitSim", Style::Default, settings);
	sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), (sf::Vector2f)app.getSize());
	//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), sf::Vector2f(app.getSize().x * 2, app.getSize().y * 2));
	//app.create(VideoMode(W, H), "CircuitSim", Style::Default, ContextSettings(0));

	W = app.getSize().x; H = app.getSize().y;
	app.setVerticalSyncEnabled(1);
	app.setFramerateLimit(60);
	srand(time(NULL));

	//std::cout << "my lcd: " << sf::VideoMode::getDesktopMode().width << ", " << sf::VideoMode::getDesktopMode().height;
	app.setPosition(sf::Vector2i(450, 100));
	//app.setPosition(sf::Vector2i(0, 0));

	/*ImGui*/
	ImGui::SFML::Init(app);//
	sf::Clock deltaClock;//

	bool RELEASE_DEBUG = 1;

	time_t frame = clock();
	bool End = 0, debugBool = 0;

	bool stimuliDisplay = 1, stimuliEndNodes = 0;

	bool releaseBool = 1, wheelReleaseBool = 1;
	bool ShiftPressed=0;

	bool Drag = 0, selectSquare = 0, mouseOnCompsBool = 0;
	bool wireBool = 0;

	int serialCompMouse = 0, serialToolMouse = 0;

	sf::Color normalCompColor(255, 255, 255), tempDimColor(150, 150, 150);

	///////////////////////////////////////////////

	/*Rayn*/ //Texture t; t.loadFromFile("Images/0 Rayn.png"); Sprite Rayn(t), Rayn2(t); Rayn.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn2.setOrigin(t.getSize().x / 2, t.getSize().y / 2); Rayn.setPosition(app.getSize().x / 2, app.getSize().y / 2); Rayn2.setPosition(app.getSize().x + 10, app.getSize().y + 10);

	//*
	bool DrawCircle = 1;
	float t_radius = 60, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
	int t_vertices = 34;
	sf::CircleShape testCircle(t_radius, t_vertices);
	testCircle.setOrigin(t_radius, t_radius);
	testCircle.setPosition(W / 2, H / 2);
	testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));//*/

	/*Grid*/
	const int gap = 15, virtualBoarder = 80;
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
		compTex[Entity::Cap].loadFromFile("assets/Images/Cap.png");
		compTex[Entity::Cur].loadFromFile("assets/Images/Cur.png");
		compTex[Entity::Dod].loadFromFile("assets/Images/Dod.png");
		compTex[Entity::Ind].loadFromFile("assets/Images/Ind.png");
		compTex[Entity::Res].loadFromFile("assets/Images/Res.png");
		compTex[Entity::SwO].loadFromFile("assets/Images/SwO.png");
		compTex[Entity::Vol].loadFromFile("assets/Images/Vol.png");
		compTex[Entity::SwC].loadFromFile("assets/Images/SwC.png");
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
	for (int c = 0; c < (Entity::noOfComps - 1); c++) {
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

	/*Circuit*/
	comp.reserve(3);
	allEnds.reserve(3);
	//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);

	/*Wires*/
	std::vector<Wire> wires; wires.reserve(3);
	//wires.emplace_back(sf::Vector2f(300, 300));

	////////////////////////////////////////////// Solve
	
	//Graph circuit;

	//std::thread printScreenTread;


	///////////////////////////////////////////////

	//clipboardxx::clipboard clipboard;

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
		bool MIntool = !!(MInTool && Mouse::getPosition(app).y < (Entity::noOfComps - 1)* c_toolColWidth);
		Occupied = 0;

		Event evnt;
		while (app.pollEvent(evnt)) {
			stimuliDisplay = 1;
			/*ImGui*/
			ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == evnt.Closed) { app.close(); End = 1; }
			if (evnt.type == evnt.Resized) {

				//view = sf::View(sf::FloatRect(view.getCenter().x - ((float)evnt.size.width) / 2, view.getCenter().y - ((float)evnt.size.height) / 2, evnt.size.width, evnt.size.height));
				view.setCenter(view.getCenter()); view.setSize((float)evnt.size.width, (float)evnt.size.height);

				/*Resting Pos of Tool Bar   (Repeated at the at of Drag{}) */ {
					toolCol.setSize(sf::Vector2f(toolCol.getSize().x, view.getSize().y));
					ToolBoxWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolBoxWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					ToolLilWinRestingPosX = view.getCenter().x - view.getSize().x / 2; ToolLilWinRestingPosY = view.getCenter().y - view.getSize().y / 2;
					for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);
				}
			}

			if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == Mouse::Left) {
				bool onNode = 0;
				static const int sensitivity = 7;
				sf::Vector2f cursorPos = cursorInSim(app);

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

					if (wireBool) wires.back().newEdge();

				}

			}
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == Mouse::Left) { releaseBool = 1; }
			if (evnt.type == evnt.MouseButtonReleased && evnt.mouseButton.button == Mouse::Middle) { wheelReleaseBool = 1; }

			if (evnt.type == evnt.KeyPressed) {
				if (evnt.key.code == Keyboard::Escape) { app.close(); End = 1; cout << "\n------------------ESC Pressed-----------------\n"; continue; }
				if (evnt.key.code == Keyboard::Delete) {
					stimuliDisplay = 1;	stimuliEndNodes = 1;
					deletef(virSerial, virSerialShift, virSprite);
				}
				if (evnt.key.code == Keyboard::W) { wireBool = !wireBool; /*cout << "\ndebug\n";*/ }
				if (evnt.key.code == Keyboard::N) { debugBool = !debugBool; /*cout << "\ndebug\n";*/ }
				if (evnt.key.code == Keyboard::P) {
					cout << "\nPrintScreen";

					//time_t print = clock();

					/*sf::Texture tex;

					tex.update(app);

					sf::Image screenshot(tex.copyToImage());

					screenshot.saveToFile("screenshot.png");*/

					std::string filepath = SaveFile("PNG (*.png)\0*.png\0");



					//screenshot = app.capture();
					//screenshot.saveToFile("screenshot.png");

					for (int c = 0; c < 1; c++) {
						//std::thread printScreenTread { printScreen }; printScreenTread.join();

						//std::async(std::launch::async, printScreen);
						if (!filepath.empty())
							printScreen(filepath);
					}
					//cout << "\n" << ((float)clock() - (float)print) / (float)CLOCKS_PER_SEC;
				}

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

						rotatef(virSerial);
						
					}
					if (evnt.key.code == Keyboard::O) {
						cout << "\nCtrl + O\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFile("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							openf(gap, filepath, virSerial, virSerialShift, virSprite);

					}
					if (evnt.key.code == Keyboard::S && evnt.key.shift) {

						std::string filepath = SaveFile("Project file (*.txt)\0*.txt\0PNG (*.png)\0*.png\0");

						if (!filepath.empty()) {

							if (filepath.back() == 't')
								savef(filepath);

							if (filepath.back() == 'g')
								printScreen(filepath);

						}

					}
					if (evnt.key.code == Keyboard::C) {
						cout << "\nCtrl + C\n";

						copyf(virSerial);

					}
					if (evnt.key.code == Keyboard::X) {
						cout << "\nCtrl + X\n"; stimuliDisplay = 1;	stimuliEndNodes = 1;

						// Copy
						{
							copyf(virSerial);
						}

						// Delete
						{
							deletef(virSerial,virSerialShift, virSprite);
						}
					}
					if (evnt.key.code == Keyboard::V) {
						cout << "\nCtrl + V\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						pastef(gap);

					}
				}
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
						comp.back().stimuli();

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
						comp.back().stimuli();

						stimuliEndNodes = 1;

					}
					else {
						mouseOnCompsBool = 0;

						/*Check every component for Mouse*/
						for (int c = 0; !mouseOnCompsBool && c < comp.size(); c++) {

							if (comp[c].bounds.contains(cursorInSim(app))) {

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
						if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; selSqr.setPosition(cursorInSim(app)); }

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
					if (Click(app, gap)) {
						stimuliDisplay = 1;

						for (int v = 0; v < virSerial.size(); v++) {

							if (comp[virSerial[v]].bounds.contains(cursorInSim(app))) {

								if (comp[virSerial[v]].serial == 5)comp[virSerial[v]].serial = 7;
								else if (comp[virSerial[v]].serial == 7) comp[virSerial[v]].serial = 5;

								comp[virSerial[v]].sprite.setTexture(compTex[comp[virSerial[v]].serial]);

								virSerial.erase(virSerial.begin() + v);
								virSprite.erase(virSprite.begin() + v);

								break;
							}

						}

					}

					if (Click(app, 3)) {
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
				if (wheelReleaseBool /*!mouseOnCompsBool && !selectSquare*/) {
					mouseHoldX = (float)Mouse::getPosition(app).x; mouseHoldY = (float)Mouse::getPosition(app).y;
					Drag = 1; mouseOnCompsBool = 0;
					viewX = view.getCenter().x, viewY = view.getCenter().y;
					verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
					horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
				}
				wheelReleaseBool = 0;
			}
			else { Drag = 0; }

			/*Continoue while hold*/
			if (mouseHoldX != (float)Mouse::getPosition(app).x || mouseHoldY != (float)Mouse::getPosition(app).y) {

				/*Follow Mouse*/
				if (mouseOnCompsBool) {
					if (!selectSquare /*&& releaseBool*/) {
						static int offSet[4][2] = {
							{0, -2},
							{2, 0},
							{0, 2},
							{-2, 0}
						};
						for (int c = 0; c < virSerial.size(); c++) {

							float tempX = (int)cursorInSim(app).x + gap * offSet[(int)comp[virSerial[c]].angle / 90][0]; ///
							float tempY = (int)cursorInSim(app).y + gap * offSet[(int)comp[virSerial[c]].angle / 90][1]; ///

							//float tempX = cursorInSim().x - mouseHoldX + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
							//float tempY = cursorInSim().y - mouseHoldY + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///

							//for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }
							for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX, tempY); }

							tempX = trim(tempX, gap);
							tempY = trim(tempY, gap);

							if (!occupiedAt(virSerial[c], sf::Vector2f(tempX, tempY))) {
								comp[virSerial[c]].x = (int)tempX; // += (were "=" before)
								comp[virSerial[c]].y = (int)tempY; // += (were "=" before)
								comp[virSerial[c]].stimuli();
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
				selSqr.setSize(sf::Vector2f(cursorInSim(app).x - selSqr.getPosition().x, cursorInSim(app).y - selSqr.getPosition().y));

				/*Selection*/
				for (int c = 0; c < comp.size(); c++) {

					bool compFound = 0;
					int v = 0;
					for (; v < virSerial.size();) {
						if (c == virSerial[v]) { compFound = 1; break; }
						else
							if (c < virSerial[v]) { break; }

						v++;
					}

					//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));

					if (comp[c].bounds.intersects(sf::FloatRect(selSqr.getPosition(), cursorInSim(app) - selSqr.getPosition()))) {
						if (!compFound) {
							//virSerial.emplace_back(c);
							virSerial.insert(virSerial.begin() + v, c);

							//virSprite.emplace_back(comp[virSerial.back()].sprite);
							virSprite.insert(virSprite.begin() + v, comp[c].sprite);
							/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
							virSprite.back().setColor(tempDimColor);*/
						}
					}
					else if (compFound) {
						virSerial.erase(virSerial.begin() + v);
						virSprite.erase(virSprite.begin() + v);
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
			if (wireBool) { stimuliDisplay = 1; wires.back().makeWire(app); }
		}

		/*cout << "\n";
		for (int v = 0; v < virSerial.size(); v++)
			cout << virSerial[v] << " ";*/
		//cout << virSerial[v] << "(" << v << "), ";

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
			ImGui::SFML::Update(app, deltaClock.restart());
			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {

					if (ImGui::MenuItem("Open...", "Ctrl + O")) {
						cout << "\nOpen";

						cout << "\nCtrl + O\n"; stimuliDisplay = 1; stimuliEndNodes = 1;

						std::string filepath = OpenFile("text file (*.txt)\0*.txt\0");

						if (!filepath.empty())
							openf(gap, filepath, virSerial, virSerialShift, virSprite);

					}
					if (ImGui::MenuItem("Save", "Ctrl + S")) { cout << "\nSave"; }
					if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {
						cout << "\nShift Save";

						std::string filepath = SaveFile("Project file (*.txt)\0*.txt\0PNG (*.png)\0*.png\0");

						if (!filepath.empty()) {

							if (filepath.back() == 't')
								savef(filepath);

							if (filepath.back() == 'g')
								printScreen(filepath);

						}

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) { app.close(); End = 1; cout << "\nEsc\n"; continue; }

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Options")) {
					
					if (ImGui::MenuItem("Handdrawn Icons")) { cout << "\nWobbly"; }
					if (ImGui::BeginMenu("Switch Themes")) {

						if (ImGui::MenuItem("Dark")) { cout << "\nDark"; }
						if (ImGui::MenuItem("Light")) { cout << "\nLight"; }

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
				for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setPosition(view.getCenter().x - view.getSize().x / 2 + ToolSprPOS[c].x, view.getCenter().y - view.getSize().y / 2 + ToolSprPOS[c].y);

			}
			testCircle.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 + 200);

			//Tool Win
			if (!Drag) toolCol.setPosition((MInTool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (Entity::noOfComps - 1)) + (!MInTool) * (t_TollWx + (ToolBoxWinRestingPosX - c_toolColWidth - t_TollWx) / (Entity::noOfComps - 1)), ToolBoxWinRestingPosY);
			if (Drag)  toolCol.setPosition(ToolBoxWinRestingPosX, ToolBoxWinRestingPosY);

			//Tool Sqr
			t_TollWx = ToolBoxLittleBox.getPosition().x;
			if (!Drag) ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / (Entity::noOfComps - 1)) + (!MIntool) * (t_TollWx + (ToolBoxWinRestingPosX - c_toolColWidth - t_TollWx) / (Entity::noOfComps - 1)), ToolLilWinRestingPosY + trim(Mouse::getPosition(app).y, c_toolColWidth));
			if (Drag)  ToolBoxLittleBox.setPosition(ToolBoxWinRestingPosX, ToolLilWinRestingPosY + trim(Mouse::getPosition(app).y, c_toolColWidth));
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

			/*Wires*/ {
				for (int c = 0; c < wires.size(); c++) wires[c].draw(app);
			}

			/*Nodes*/ {
				for (int e = 0; e < allEndCircles.size(); e++) { app.draw(allEndCircles[e]); }
			}

			/*Boarders*/ {
				for (int v = 0; v < virSerial.size(); v++) { app.draw(comp[virSerial[v]].boarder); }
			}

			/*Virtual Sprites*/ {
				if (Occupied) for (int v = 0; v < virSprite.size(); v++) app.draw(virSprite[v]);
			}

			/*Selection Sqraure*/ {
				if (selectSquare && !releaseBool) app.draw(selSqr);
			}

			/*Tool Win*/ {
				if (MInTool) {
					app.draw(toolCol);
					for (int c = 0; c < (Entity::noOfComps - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
					for (int c = 0; c < (Entity::noOfComps - 1); c++) { app.draw(ToolSpr[c]); }
				}
				else { for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setColor(sf::Color(255, 255, 255, 0)); }

				if (MIntool) app.draw(ToolBoxLittleBox);
			}

			/*ImGui*/ {
				if (DrawCircle) app.draw(testCircle);
				ImGui::SFML::Render(app);//Last Thing to render
			}

			app.display();
		}


		app.setTitle("CircuitSim   " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
		frame = clock();

		stimuliDisplay = 0; stimuliEndNodes = 0;
	}
	


	stimuliDisplay = 1; stimuliEndNodes = 1;
	
	/*ImGui*/
	ImGui::SFML::Shutdown();
	std::cin.get();
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
			if (cc == 1) { /*Rear*/  tempEnd = comp[c].getEndPos(); }

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

void printScreen(const std::string& filepath) {

	//try
	//{
	//	cout << filepath;
	//	int len;
	//	int slength = (int)filepath.length() + 1;
	//	len = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), slength, 0, 0);
	//	wchar_t* buf = new wchar_t[len];
	//	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), slength, buf, len);
	//	std::wstring r(buf);
	//	delete[] buf;
	//	const wchar_t* widecstr = r.c_str();

	//	//ShellExecute(NULL, NULL, L"Saved-Images", NULL, NULL, SW_SHOWNORMAL);
	//	ShellExecute(NULL, NULL, widecstr, NULL, NULL, SW_SHOWNORMAL);
	//}
	//catch (const std::exception&)
	//{
	//	cout << "Shell connot open floder  \"Saved - Images\"";
	//}

	sf::Image OutputImage;
	sf::FloatRect compBound;
	sf::FloatRect ABCD;

	if (comp.size() != 0) {
		compBound = comp[0].bounds;
		ABCD = compBound;

		for (int c = 0; c < comp.size(); c++) {
			compBound = comp[c].bounds;
			if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
			if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
			if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
			if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
		}
		/*for (int c = 0; c < comp.size(); c++) {
			sf::Vector2f tempEndNode = endNodePos(comp[c]);
			if (tempEndNode.x < A) A = tempEndNode.x;
			if (tempEndNode.x > B) B = tempEndNode.x;
			if (tempEndNode.y < C) C = tempEndNode.y;
			if (tempEndNode.y > D) D = tempEndNode.y;
		}*/
		OutputImage.create(abs(ABCD.width) + 30, abs(ABCD.height) + 30);
		//cout << "\n" << "S: " << abs(A - B) << ", " << abs(C - D);
		//cout << "\n" << A << ", " << B << ", " << C << ", " << D;
	}
	else { OutputImage.create(10, 10); }

	for (int c = 0; c < comp.size(); c++) {
		sf::Vector2f tempEndNode = comp[c].getEndPos();
		compBound = comp[c].bounds;

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

		float OffX = compBound.left - ABCD.left - !rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;
		float OffY = compBound.top - ABCD.top - rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;

		for (int j = rotBool * fakeGap; j < tempCompImg.getSize().y - rotBool * fakeGap; j++) {
			for (int i = !rotBool * fakeGap; i < tempCompImg.getSize().x - !rotBool * fakeGap; i++) {
				if (tempCompImg.getPixel(i, j).a == 0) continue;
				//std::async(std::launch::async, &sf::Image::setPixel, &OutputImage, OffX + i, OffY + j, tempCompImg.getPixel(i, j));

				OutputImage.setPixel(OffX + i, OffY + j, tempCompImg.getPixel(i, j));
			}
		}

	}

	//int picNo = 0;
	//std::string picDir = "Saved-Images/Untitled-", picType = ".png";
	//sf::Image test;
	//while (test.loadFromFile(picDir + std::to_string(picNo++) + picType)) {
	//	;
	//	//cout << "\n" << picNo;
	//}
	//OutputImage.saveToFile(picDir + std::to_string(picNo - 1) + picType);

	OutputImage.saveToFile(filepath);

}

void openf(int Gap, const std::string& filepath, std::vector<int>& vir, std::vector<int>& virShft, std::vector<sf::Sprite>& virSpr) {

	vir.clear();
	virSpr.clear();
	virShft.clear();


	//std::ifstream input(OpenFile("text file (*.txt)\0*.txt\0"));
	std::ifstream input(filepath);

	/*int fileNo = 0;
	std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
	std::ifstream input(fileDir + std::to_string(fileNo) + fileType);
	while (input.good()) {
		input.close();
		input.open(fileDir + std::to_string(++fileNo) + fileType);
	}
	input.open(fileDir + std::to_string(fileNo - 1) + fileType);*/

	comp.clear();

	int no = 0;
	input >> no;
	for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
		input >> S >> X >> Y >> A;
		comp.emplace_back(S % (Entity::noOfComps + 1), trim(X, Gap), trim(Y, Gap), ((A % 360) / 90) * 90);
	}
	input.close();

}

void savef(const std::string& file) {

	cout << "\nCtrl + S\n";

	//ShellExecute(NULL, NULL, L"Saved-Projects", NULL, NULL, SW_SHOWNORMAL);

	std::ofstream output;

	//int fileNo = 0;
	//std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
	//std::ifstream test(fileDir + std::to_string(fileNo) + fileType);
	//while (test.good()) {
	//	test.close();
	//	test.open(fileDir + std::to_string(++fileNo) + fileType);
	//}
	//output.open(fileDir + std::to_string(fileNo) + fileType);

	//output.open(SaveFile("text file (*.txt)\0*.txt\0"));
	
	//if (file.empty()) return;
	
	output.open(file);

	std::string tempStr;
	tempStr = std::to_string((int)comp.size()) + "\n";
	int size = (int)comp.size();
	for (int c = 0; c < size; c++) {
		tempStr += std::to_string(comp[c].serial) + "\t" + std::to_string((int)comp[c].x) + "\t" + std::to_string((int)comp[c].y) + "\t" + std::to_string((int)comp[c].angle) + "\n";
	}
	output << tempStr;
	output.close();
}

void copyf(const std::vector<int>& vir) {

	std::ofstream output;
	std::string tempStr(std::to_string((int)vir.size()) + "\n");

	int size = (int)vir.size();
	for (int c = 0; c < size; c++) {
		tempStr += std::to_string(comp[vir[c]].serial) + "\t" + std::to_string((int)comp[vir[c]].x) + "\t" + std::to_string((int)comp[vir[c]].y) + "\t" + std::to_string((int)comp[vir[c]].angle) + "\n";
	}

	//clipboard << tempStr;
	sf::Clipboard::setString(tempStr);

}

void pastef(int Gap) {

	std::string inString;
	std::vector<int> integers; integers.reserve(9);
	//clipboard >> inString;
	inString = sf::Clipboard::getString();


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
		comp.emplace_back(S % (Entity::noOfComps + 1), trim(X, Gap), trim(Y, Gap), ((A % 360) / 90) * 90);
	}

}

void rotatef(const std::vector<int>& vir) {

	for (int v = 0; v < vir.size(); v++) {
		comp[vir[v]].angle += 90;
		comp[vir[v]].angle -= (int)(comp[vir[v]].angle / 360) * 360;
		comp[vir[v]].stimuli();
	}

}

void deletef(std::vector<int>& vir, std::vector<int>& virShft, std::vector<sf::Sprite>& virSpr) {

	int diff = 0;

	for (int v = 0; v < vir.size(); v++)
		comp.erase(comp.begin() + (vir[v] - diff++)); //assuming vir or virSerial is sorted(ascendingly)

	vir.clear();
	virShft.clear();
	virSpr.clear();

	/*while (0 < vir.size()) {
		if (comp.size() > 0) comp.erase(comp.begin() + vir[0]);
		if (virSpr.size() > 0) virSpr.erase(virSpr.begin());

		for (int c = 1; c < vir.size(); c++) {
			if (vir[c] > vir[0]) vir[c]--;
		}

		vir.erase(vir.begin());
	}*/
}


float trim(float num, int wrt) {
	return num - (int)num % wrt;
}

sf::Vector2f cursorInSim(const sf::RenderWindow& App) {
	return App.mapPixelToCoords(sf::Mouse::getPosition(App));
}

bool Click(const sf::RenderWindow& App, int Sensitivity) {
	return (((float)clock() - (float)click) < 100) && !!(!Mouse::isButtonPressed(Mouse::Left) && abs(mouseHoldX - (float)Mouse::getPosition(App).x) <= Sensitivity && abs(mouseHoldY - (float)Mouse::getPosition(App).y) <= Sensitivity);
}

bool occupiedAt(int Index, const sf::Vector2f& At) {

	int noCount = 0;
	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode

		if (At.x == comp[c].x && At.y == comp[c].y) {
			if (comp[Index].angle != comp[c].angle) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

		if (At.x == comp[c].getEndPos().x && At.y == comp[c].getEndPos().y) {
			if (abs((int)(comp[Index].angle - comp[c].angle)) != 180) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

	}
	if (!!noCount) { Occupied = 0; return 0; } //if (noCount == 1) { Occupied = 0; return 0; }


	Entity indexEntity(comp[Index]);
	indexEntity.x = At.x; indexEntity.y = At.y;
	indexEntity.stimuli();
	const sf::Vector2f end(indexEntity.getEndPos());
	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - end.x) >= 200 || abs(comp[c].y - end.y) >= 200) continue; // gap-hardcode

		if (end.x == comp[c].x && end.y == comp[c].y) {
			if (abs((int)(comp[Index].angle - comp[c].angle)) != 180) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

		if (end.x == comp[c].getEndPos().x && end.y == comp[c].getEndPos().y) {
			if (comp[Index].angle != comp[c].angle) {
				noCount++;
			}
			else {
				Occupied = 1; return 1;
			}
		}

	}
	if (!!noCount) { Occupied = 0; return 0; } //if (noCount == 1) { Occupied = 0; return 0; }

	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode
		if (indexEntity.bounds.intersects(comp[c].bounds)) { Occupied = 1; return 1; }

	}

	Occupied = 0; return 0;
}

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