#pragma once
/*
taskkill /F /IM Circiut_Sim.exe
*/

//#include "imgui.h"
//#include "imgui-SFML.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "Screen_Entity.h"
#include "Screen_Graph.h"

using namespace sf;
using std::cout; using std::to_string; using std::vector; using std::fstream;

float trim(float num, int wrt);
sf::Vector2f endNodePos(Entity Comp);
sf::Vector2f cursorInSim();
bool occupiedAt(int Index, sf::Vector2f At);
bool compIn(Entity Comp, sf::Vector2f Ini, sf::Vector2f Fin);
void getBounds(Entity Comp, int arr[4]);

//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
sf::RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Default, ContextSettings(0));
sf::View view(sf::Vector2f(W / 2, H / 2), sf::Vector2f(W, H));
vector<Entity> comp;

bool Occupied = 0;

int main() {

	Graph graph;

	graph.Vector.emplace_back(0);
	graph.Vector.emplace_back(1);
	graph.Vector.emplace_back(2);
	graph.Vector.emplace_back(3);
	graph.Vector.emplace_back(4);
	graph.Vector.emplace_back(5);
	graph.Vector.emplace_back(6);


	graph.Vector[0].neighbors.emplace_back(&graph.Vector[5]);
	graph.Vector[0].neighbors.emplace_back(&graph.Vector[4]);
	graph.Vector[0].neighbors.emplace_back(&graph.Vector[1]);
	graph.Vector[5].neighbors.emplace_back(&graph.Vector[3]);
	graph.Vector[0].neighbors.emplace_back(&graph.Vector[6]);







	








	//ImGui::SFML::Init(app);//
	//sf::Clock deltaClock;//

	W = app.getSize().x; H = app.getSize().y;
	app.setVerticalSyncEnabled(1);
	app.setFramerateLimit(60);
	srand(time(NULL));


	time_t frame = clock();
	bool End = 0, Pause = 0, printScreen = 0;
	bool debugBool = 0;

	/*Flags*/
	bool releaseBool = 1;

	bool Click = 0, Drag = 0, selectSquare = 0, mouseOnCompsBool = 0;
	bool onceOptComp = 0, delComp = 0, rotComp = 0;

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

	////////////////////////////////////////////// Grid
	int gap = 15, virtualBoarder = 80;
	vector<sf::RectangleShape> vLines;
	vLines.reserve((W + 2 * virtualBoarder) / gap + 2);

	sf::Vector2f tempVect(2, H + 2 * virtualBoarder);
	for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
		vLines.emplace_back(tempVect);
	}

	sf::Color tempColor(100, 105, 110, 20);
	for (int c = 0; c < vLines.size(); c++) {
		vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);
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
		hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);
		tempColor.a = 20 + (c % 5 == 0) * 15;
		hLines[c].setFillColor(tempColor);
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

	sf::Sprite ToolSpr[7];
	sf::Vector2f ToolSprPOS[7];
	for (int c = 0; c < 7; c++) {
		ToolSpr[c].setTexture(compTex[c]);
		ToolSpr[c].setOrigin(compTex[c].getSize().x / 2, 0);
		//compSpr[c].setRotation(45);
		ToolSprPOS[c].x = ToolBoxWin.getSize().x / 2;
		ToolSprPOS[c].y = c * ToolBoxWin.getSize().x + (int)(ToolBoxWin.getSize().x - compTex[c].getSize().y) / 2;
		ToolSpr[c].setPosition(ToolSprPOS[c].x, ToolSprPOS[c].y);
	}

	////////////////////////////////////////////// VirutalComps
	vector<int> virSerial;
	vector<sf::Sprite> virSprite;
	virSerial.reserve(8);
	virSprite.reserve(8);

	sf::RectangleShape selSqr;
	sf::Vector2f selSqrInital;
	selSqr.setFillColor(sf::Color(0, 0, 0,0));
	selSqr.setOutlineThickness(1.0f);
	selSqr.setOutlineColor(sf::Color(255, 0, 255));


	////////////////////////////////////////////// Tool
	
	comp.reserve(8);
	//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);


	

	fstream testFile;

	




	///////////////////////////////////////////////
	while (!End) {
		
		graph.setGraph();

		float mouseHoldX = Mouse::getPosition(app).x, mouseHoldY = Mouse::getPosition(app).y;
		float viewX = view.getCenter().x, viewY = view.getCenter().y;
		float verX = vLines[0].getPosition().x, verY = vLines[0].getPosition().y;
		float horX = hLines[0].getPosition().x, horY = hLines[0].getPosition().y;

		int verBrightCount = 5, horBrightCount = 5;

		while (app.isOpen() && !End) {



			W = app.getSize().x; H = app.getSize().y;

			float t_TollWx = ToolBoxWin.getPosition().x;
			bool MInTool = !!(0 <= Mouse::getPosition(app).x && Mouse::getPosition(app).x <= t_TollWX);
			bool MIntool = !!(MInTool && Mouse::getPosition(app).y < 7 * t_TollWX);
			delComp = 0; rotComp = 0;
			Occupied = 0;


			Event evnt;
			while (app.pollEvent(evnt)) {

				//ImGui::SFML::ProcessEvent(evnt);

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
					//if (evnt.key.code == Keyboard::R) { cout << "\n------------------   Reset   -----------------\n"; goto END; }
					if (evnt.key.code == Keyboard::P) { printScreen = 1; }
					if (evnt.key.code == Keyboard::N) { debugBool = !debugBool; /*cout << "\ndebug\n";*/ }

					/*int difr = 10;
					if (evnt.key.code == Keyboard::Up) { view.setCenter(view.getCenter().x, view.getCenter().y - difr); }
					if (evnt.key.code == Keyboard::Down) { view.setCenter(view.getCenter().x, view.getCenter().y + difr); }
					if (evnt.key.code == Keyboard::Right) { view.setCenter(view.getCenter().x + difr, view.getCenter().y); }
					if (evnt.key.code == Keyboard::Left) { view.setCenter(view.getCenter().x - difr, view.getCenter().y); }*/

					if (evnt.key.code == Keyboard::Delete) { delComp = 1; }

				}
			}
			///////////////////////////////////////////////
			if ((Keyboard::isKeyPressed(Keyboard::R)) && (Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift))) { rotComp = 1; /*cout << "Rotate";*/ }
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


			// ----------------------------------------	Options
			/*Mouse Hold*/
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (releaseBool) {
					releaseBool = 0;
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
					}
					else {
						mouseOnCompsBool = 0;

						/*Check every component for Mouse*/
						for (int c = 0; !mouseOnCompsBool && c < comp.size(); c++) {
							float tempCompX = comp[c].x, tempCompY = comp[c].y;
							
							/*Dealing with Origin*/
							int a = 0, b = 20, d = 75;
							int bounds[4], i = comp[c].angle;
							
							getBounds(comp[c], bounds);

							if ((tempCompX - bounds[0] < cursorInSim().x) && (cursorInSim().x < tempCompX + bounds[1])) {
								if ((tempCompY - bounds[2] < cursorInSim().y) && (cursorInSim().y < tempCompY + bounds[3])) {
									virSerial.emplace_back(c);

									for (int v = 0; v < virSerial.size(); v++) {
										virSprite.emplace_back(comp[virSerial[0]].sprite);
										virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x/2, virSprite.back().getTexture()->getSize().y / 2);
										virSprite.back().setColor(tempDimColor);
									}

									mouseOnCompsBool = 1; Drag = 0;
								}

							}
						}

						//////////// Urgent need of enums , State Machine
						if (!mouseOnCompsBool && selectSquare) { selSqrInital = cursorInSim(); }
						
						/*Drag Background*/
						if (!mouseOnCompsBool && !selectSquare) {
							Drag = 1; mouseOnCompsBool = 0;
							viewX = view.getCenter().x, viewY = view.getCenter().y;
							verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
							horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
							//wxyz = verBrightCount;
						}
					}
				}
				
				if (virSerial.size() > 0) mouseOnCompsBool = 1;

				/*Once while hold*/
				//cout << "\n" << mouseOnCompsBool << " " << rotComp;
				if (mouseOnCompsBool && rotComp) {
					if (onceOptComp) {
						for (int v = 0; v < virSerial.size(); v++) {
							comp[virSerial[v]].angle += 90;
							comp[virSerial[v]].angle -= (int)(comp[virSerial[v]].angle / 360) * 360;
						}
					}
					onceOptComp = 0;
				}
				else if (mouseOnCompsBool && delComp) {
					if (onceOptComp) {
						for (int v = 0; v < virSerial.size(); v++) {
							if (comp.size() > 0) comp.erase(comp.begin() + virSerial[v]);
							if (virSerial.size() > 0) virSerial.erase(virSerial.begin() + v);
							if (virSprite.size() > 0) virSprite.erase(virSprite.begin() + v);
						}

					}
					onceOptComp = 0;
					mouseOnCompsBool = 0;
				}
				else onceOptComp = 1;
			}
			else {
				Drag = 0; mouseOnCompsBool = 0;
				/*Click*/
				if (abs(mouseHoldX - (float)Mouse::getPosition(app).x) <= gap && abs(mouseHoldY - (float)Mouse::getPosition(app).y) <= gap) {
					for (int v = 0; v < virSerial.size(); v++) {
						int tempCompClick = comp[virSerial[v]].serial;
						comp[virSerial[v]].serial = (tempCompClick == 5) * 7 + (tempCompClick == 7) * 5 + (tempCompClick != 5 && tempCompClick != 7) * (tempCompClick);
						comp[virSerial[v]].sprite.setTexture(compTex[comp[virSerial[v]].serial]);
					}

				}

				virSprite.clear();  /////
				virSerial.clear();  /////

			/*ZZzzzz Recolor back to normal    & clear serials
			if (virSerial.size() != 0) {
				for (int v = 0; v < virSerial.size() && comp.size() != 0; v++) {
					cout << "\nChanging" << virSerial[v];
					comp[virSerial[v]].sprite.setColor(normalCompColor);
				}
			}*/
			}



			/*Continoue while hold*/
			if (mouseOnCompsBool && (mouseHoldX != (float)Mouse::getPosition(app).x || mouseHoldY != (float)Mouse::getPosition(app).y)) {
				/*Follow Mouse*/
				int tempRotArr[4][2] = {
					{0, -2},
					{2, 0},
					{0, 2},
					{-2, 0}
				};
				for (int c = 0; c < virSerial.size(); c++) {

					float tempX = cursorInSim().x + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][0]; ///
					float tempY = cursorInSim().y + gap * tempRotArr[(int)comp[virSerial[c]].angle / 90][1]; ///
					for (int v = 0; v < virSprite.size(); v++) { virSprite[0].setPosition(tempX + v * gap, tempY + v * gap); }

					tempX = trim(tempX, gap);
					tempY = trim(tempY, gap);

					if (!occupiedAt(virSerial[c], sf::Vector2f(tempX, tempY))) {
						comp[virSerial[c]].x = tempX;
						comp[virSerial[c]].y = tempY;
					}
					//cout << "\nYYAAAHHH";
				}
			}

			/*Select Sqr*/
			if (selectSquare && !releaseBool) {

				/*Sel Sqr*/
				selSqr.setPosition(selSqrInital.x, selSqrInital.y);
				selSqr.setSize(sf::Vector2f(cursorInSim().x - selSqrInital.x, cursorInSim().y - selSqrInital.y));

				/*Selection*/
				for (int c = 0; c < comp.size(); c++) {

					bool compFound = 0;
					int v = 0;
					for (; v < virSerial.size(); v++) {
						if (c == virSerial[v]) { compFound = 1; break; }
					};

					if (compIn(comp[c], selSqrInital, cursorInSim())) {
						if (!compFound) virSerial.emplace_back(c);
					}
					else {
						if (compFound) virSerial.erase(virSerial.begin() + v);
					}
				}
			}

			//cout << "\n" << virSerial.size() << " " << virSprite.size();

			/*PrintScreen*/
			if (printScreen) {
				printScreen = 0;
				sf::Image OutputImage;
				int compArr[4];
				int A = 0, B = 0, C = 0, D = 0;

				if (comp.size() != 0) {
					getBounds(comp[0], compArr);
					A = comp[0].x - compArr[0], B = comp[0].x + compArr[1], C = comp[0].y - compArr[2], D = comp[0].y + compArr[3]; // Borders

					for (int c = 0; c < comp.size(); c++) {
						getBounds(comp[c], compArr);
						if (comp[c].x - compArr[0] <= A) A = comp[c].x - compArr[0];
						if (comp[c].x + compArr[1] >= B) B = comp[c].x + compArr[1];
						if (comp[c].y - compArr[2] <= C) C = comp[c].y - compArr[2];
						if (comp[c].y + compArr[3] >= D) D = comp[c].y + compArr[3];
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

				//cout << "\n" << "S: " << OutputImage.getSize().x << ", " << OutputImage.getSize().y;

				for (int c = 0; c < comp.size(); c++) {
					sf::Vector2f tempEndNode = endNodePos(comp[c]);
					getBounds(comp[c], compArr);

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

					//float OffX = (comp[c].x + (compArr[1] - compArr[0]) / 2) - A; float OffY = (comp[c].y + (compArr[3] - compArr[2]) / 2) - C;

					bool rotBool = ((((int)comp[c].angle) / 90) % 2 == 1);
					//cout << "\nrotBool " << rotBool;
					
					int fakeGap = 15 - (comp[c].serial == 5) * 5;

					float OffX = comp[c].x - compArr[0] - A - !rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + gap;
					float OffY = comp[c].y - compArr[2] - C -  rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + gap;
					//cout << "\n" << OffX << ", " << OffY; 
					
					for (int j = rotBool * fakeGap; j < tempCompImg.getSize().y - rotBool * fakeGap; j++) {
						for (int i = !rotBool * fakeGap; i < tempCompImg.getSize().x - !rotBool * fakeGap; i++) {
							if (tempCompImg.getPixel(i, j).a == 0) continue;

							//cout << "\n" << j << ", " << i;
							OutputImage.setPixel(OffX + i, OffY + j, tempCompImg.getPixel(i, j));
							//OutputImage.setPixel(i, j, tempCompImg.getPixel(i, j));
						}
					}

				}
				//cout << "\n" << "S: " << OutputImage.getSize().x << ", " << OutputImage.getSize().y;

				OutputImage.saveToFile("Saved-Images/0 OuputImage.png");
			}

			if (Drag) {
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
						tempColor.a = 20 + ((c + verBrightCount) % 5 == 0) * 15;
						vLines[c].setFillColor(tempColor);
					}


					horBrightY -= hLines[horBrightCount].getPosition().y;
					if (horBrightY > gap * 0.9) horBrightCount--; else if (horBrightY < -gap * 0.9) horBrightCount++;
					horBrightCount = abs((horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1)) % 6;
					//cout << "\n" << horBrightCount << "\n";
					for (int c = 0; c < hLines.size(); c++) {
						tempColor.a = 20 + ((c + horBrightCount) % 5 == 0) * 15;
						hLines[c].setFillColor(tempColor);
					}

					/*Resting Pos of Tool Bar*/
					ToolBoxWinRestingPosX = newHorX + virtualBoarder; ToolBoxWinRestingPosY = newVerY + virtualBoarder;
					ToolLilWinRestingPosX = newHorX + virtualBoarder; ToolLilWinRestingPosY = newVerY + virtualBoarder;
					for (int c = 0; c < 7; c++) ToolSpr[c].setPosition(newHorX + virtualBoarder + ToolSprPOS[c].x, newVerY + virtualBoarder + ToolSprPOS[c].y);

				}


			// ----------------------------------------	Update

			if (graph.win->isOpen()) graph.updateWin();

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


			/*Tool Win*/ ToolBoxWin.setPosition((MInTool)* (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / 7) + (!MInTool) * (t_TollWx + (ToolBoxWinRestingPosX - t_TollWX - t_TollWx) / 7), ToolBoxWinRestingPosY);

			/*Tool Sqr*/ t_TollWx = ToolBoxLittleBox.getPosition().x;
			ToolBoxLittleBox.setPosition((MIntool)* (t_TollWx + (ToolBoxWinRestingPosX + 0 - t_TollWx) / 7) + (!MIntool) * (t_TollWx + (ToolBoxWinRestingPosX - t_TollWX - t_TollWx) / 7), ToolLilWinRestingPosY + trim(Mouse::getPosition(app).y, (int)ToolBoxLittleBox.getSize().x));
			if (MIntool) serialToolMouse = (int)(Mouse::getPosition(app).y / ToolBoxLittleBox.getSize().x); else serialToolMouse = 0;
			
			// ----------------------------------------	Draw
			{
				app.setView(view);
				if (!Pause) {
					app.clear(sf::Color(23, 24, 25));
					//app.draw(Rayn);
					//app.draw(Rayn2);

					/*grid*/ {
						for (int c = 0; c < vLines.size(); c++) { app.draw(vLines[c]); }
						for (int c = 0; c < hLines.size(); c++) { app.draw(hLines[c]); }
					}

					/*comp*/ {
						for (int c = 0; c < comp.size(); c++) { comp[c].draw(app, gap); }
					}


					if(Occupied) for (int v = 0; v < virSprite.size(); v++) { app.draw(virSprite[v]); }

					//if(DrawCircle) app.draw(testCircle);

					selectSquare = debugBool;

					if (selectSquare && !releaseBool)app.draw(selSqr);

					/*Tool Win*/ {
						if (MInTool) {
							app.draw(ToolBoxWin);
							for (int c = 0; c < 7; c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
							for (int c = 0; c < 7; c++) { app.draw(ToolSpr[c]); }
						}
						else { for (int c = 0; c < 7; c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, 0)); } }
						
						if (MIntool) app.draw(ToolBoxLittleBox);
					}


					/*ImGui*/
					//ImGui::SFML::Render(app);//Last Thing to render

					app.display();
				}
			}

			app.setTitle("CircuitSIm   " + to_string((float)(CLOCKS_PER_SEC / ((float)clock() - (float)frame))));
			frame = clock();


			printScreen = 0;
		}

	END:
		;
		Pause = 0; printScreen = 0;
	}

	//ImGui::SFML::Shutdown();
	system("pause");
	return 0;
}

float trim(float num, int wrt) {
	return num - (int)num % wrt;
}

sf::Vector2f endNodePos(Entity Comp) {
	return sf::Vector2f(Comp.x - 75 * (int)sin(Comp.angle * DegToRad), Comp.y + 75 * (int)cos(Comp.angle * DegToRad));
}

sf::Vector2f cursorInSim() {
	return sf::Vector2f(Mouse::getPosition(app).x + view.getCenter().x - W / 2, Mouse::getPosition(app).y + view.getCenter().y - H / 2);
}

bool occupiedAt(int Index, sf::Vector2f At) {
	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 100 || abs(comp[c].y - At.y) >= 100) continue;
		if (At.x == comp[c].x && At.y == comp[c].y) {
			if (comp[Index].angle != comp[c].angle) {
				Occupied = 0; return 0;
			}
			else {
				Occupied = 1; return 1;
			}
		}
		if (At.x == endNodePos(comp[c]).x && At.y == endNodePos(comp[c]).y) {
			if (abs(comp[Index].angle - comp[c].angle) != 180) {
				Occupied = 0; return 0;
			}
			else {
				Occupied = 1; return 1;
			}
		}

	}

	for (int c = 0; c < comp.size(); c++) {
		if (c == Index || abs(comp[c].x - At.x) >= 100 || abs(comp[c].y - At.y) >= 100) continue;

		int TempArr[4] = { 0,0,0,0 };
		float tempCompX = comp[c].x, tempCompY = comp[c].y;
		getBounds(comp[c], TempArr);

		if ((tempCompX - TempArr[0] < At.x) && (At.x < tempCompX + TempArr[1])) {
			if ((tempCompY - TempArr[2] < At.y) && (At.y < tempCompY + TempArr[3])) {
				Occupied = 1; return 1;
			}
		}

	}

	Occupied = 0;
	return 0;
}

bool compIn(Entity Comp, sf::Vector2f Ini, sf::Vector2f Fin) {
	
	float tempCompX = Comp.x, tempCompY = Comp.y;
	int bounds[4];
	getBounds(Comp, bounds);
	float A = 0, B = 0, C = 0, D = 0;

	if (Ini.x <= Fin.x) { A = Ini.x; B = Fin.x; } else { B = Ini.x; A = Fin.x; }
	if (Ini.y <= Fin.y) { C = Ini.y; D = Fin.y; } else { D = Ini.y; C = Fin.y; }
	
	if ((A <= tempCompX - bounds[0]) && (tempCompX + bounds[1] <= B)) {
		if ((C <= tempCompY - bounds[2]) && (tempCompY + bounds[3] <= D)) {
			return 1;
		}
	}
	
	return 0;
}

void getBounds(Entity Comp, int arr[4]) {
	
	float tempCompX = Comp.x, tempCompY = Comp.y;

	/*Dealing with Origin*/
	int a = 0, b = 15, d = 75;
	int A = 15, B = 15, C = 0, D = 75, i = Comp.angle;
	i %= 360;
	if ((i / 90) % 2 == 0) {
		A = b; B = b;
		if ((i / 90) == 0) { C = a; D = d; }
		else { C = d; D = a; }
	}
	else {
		C = b; D = b;
		if ((i / 90) - 1 == 0) { A = d; B = a; }
		else { A = a; B = d; }
	}

	arr[0] = A; arr[1] = B; arr[2] = C; arr[3] = D;
}