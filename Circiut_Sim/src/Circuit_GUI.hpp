// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
#pragma once

#include <iostream>
#include <windows.h> //To Open file(txt/PNG) just after creating(Save as...) it

#include "SFML/Graphics.hpp"
#include "Circuit_Entity.hpp"
#include "Resource_Manager.hpp"
//#include "Circuit_Global.h"

//extern sf::Texture compTex[8];
extern std::vector<sf::Texture> compTex;

namespace CircuitGUI {

	/*Constants*/
	const int gap = 15;
	const sf::Vector2f zero(0, 0);
	const sf::Color normalCompColor(230, 230, 230);

	bool darkLightMode = false;
	sf::Color tempDimColor(60, 60, 60/*150, 150, 150*/);

	/*Textures*/
	//void loadTextures()
	Resource Resource_Logo;
	Resource Resource_Font;
	std::vector<Resource> Resource_Images;

	sf::Event evnt;
	sf::RenderWindow app;
	//renderWinInit();
	sf::View view;
	//viewInit();


	/*Cursor*/
	float mouseHoldX, mouseHoldY;
	time_t click = clock(); // Time passed since Click
	bool Click(int Sensitivity = 3) {
		return (((float)clock() - (float)click) < 100) && !!(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && abs(mouseHoldX - (float)sf::Mouse::getPosition(app).x) <= Sensitivity && abs(mouseHoldY - (float)sf::Mouse::getPosition(app).y) <= Sensitivity);
	}
	sf::Vector2f cursorInSim() {
		return app.mapPixelToCoords(sf::Mouse::getPosition(app));
	}
	sf::Vector2f onScreen(float X, float Y) {
		return sf::Vector2f(view.getCenter().x - view.getSize().x / 2 + X, view.getCenter().y - view.getSize().y / 2 + Y);
		//return app.mapPixelToCoords(sf::Vector2i((int)X, (int)Y));
	}
	float trim(float num, int wrt = gap) {
		return num - (int)num % wrt;
	}
	sf::Vector2f trim(sf::Vector2f vec, int wrt = gap) {
		vec.x -= (int)vec.x % wrt;
		vec.y -= (int)vec.y % wrt;

		return vec;
	}


	/*Grid*/
	int virtualBoarder = 80;
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;
	sf::Color gridColor(100, 105, 110, 20);
	sf::Color backColor(23, 24, 25);
	void colorGrid() {
		if (darkLightMode)CircuitGUI::gridColor = { 212, 232, 247, 50 };
		else CircuitGUI::gridColor = { 100, 105, 110, 20 }; //R,G,B,a
	}
	void colorBackground() {
		if (darkLightMode) CircuitGUI::backColor = { 36, 133, 202 };
		else CircuitGUI::backColor = { 23, 24, 25 }; //R,G,B
	}
	void drawGrid() {
		for (int c = 0; c < vLines.size(); c++) { app.draw(vLines[c]); }
		for (int c = 0; c < hLines.size(); c++) { app.draw(hLines[c]); }
	}
	//void initializeGrid()


	/*Drag*/
	bool dragBool = 0;
	float viewX = 0, viewY = 0;
	float verX = 0, verY = 0;
	float horX = 0, horY = 0;
	int verBrightCount = 5, horBrightCount = 5;
	void iniDrag() {
		mouseHoldX = (float)sf::Mouse::getPosition(app).x; mouseHoldY = (float)sf::Mouse::getPosition(app).y;
		viewX = view.getCenter().x, viewY = view.getCenter().y;
		verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
		horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
	}
	void Drag() {
		view.setCenter(sf::Vector2f(viewX + mouseHoldX - (float)sf::Mouse::getPosition(app).x, viewY + mouseHoldY - (float)sf::Mouse::getPosition(app).y));
		float newVerY = verY + mouseHoldY - (float)sf::Mouse::getPosition(app).y;
		float newHorX = horX + mouseHoldX - (float)sf::Mouse::getPosition(app).x;


		float verBrightX = vLines[verBrightCount].getPosition().x;
		for (int c = 0; c < vLines.size(); c++) {
			vLines[c].setPosition(trim(newHorX) + c * gap, newVerY);
		}
		float horBrightY = hLines[horBrightCount].getPosition().y;
		for (int c = 0; c < hLines.size(); c++) {
			hLines[c].setPosition(newHorX, trim(newVerY) + c * gap);
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


		horBrightY -= hLines[horBrightCount].getPosition().y;
		if (horBrightY > gap * 0.9) horBrightCount--; else if (horBrightY < -gap * 0.9) horBrightCount++;
		horBrightCount = abs((horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1)) % 6;
		//cout << "\n" << horBrightCount << "\n";
	}
	void colorBrightLineGrid() {
		for (int c = 0; c < vLines.size(); c++) {
			gridColor.a = 20 + ((c + verBrightCount) % 5 == 0) * 15;
			vLines[c].setFillColor(gridColor);
		}
		for (int c = 0; c < hLines.size(); c++) {
			gridColor.a = 20 + ((c + horBrightCount) % 5 == 0) * 15;
			hLines[c].setFillColor(gridColor);
		}
	}


	/*Vectors*/
	std::vector<Entity> comp;
	std::vector<int> virSerial;
	std::vector<sf::Sprite> virSprite;
	std::vector<int> virSerialShift;
	std::vector<sf::Vector2f> allEnds;
	std::vector<sf::CircleShape> allEndCircles;
	void drawComp() {
		for (int c = 0; c < CircuitGUI::comp.size(); c++) { CircuitGUI::comp[c].draw(CircuitGUI::app); }
	}
	void drawVirSprites() {
		for (int v = 0; v < CircuitGUI::virSprite.size(); v++)
			CircuitGUI::app.draw(CircuitGUI::virSprite[v]);
	}
	void drawNodes() {
		for (int e = 0; e < CircuitGUI::allEndCircles.size(); e++) { CircuitGUI::app.draw(CircuitGUI::allEndCircles[e]); }
	}
	void drawBoarders() {
		for (int v = 0; v < CircuitGUI::virSerial.size(); v++)
			CircuitGUI::app.draw(CircuitGUI::comp[CircuitGUI::virSerial[v]].boarder);
	}
	void updateAllEnds() {

		allEnds.clear();

		int nodeCount = 0;
		for (int c = 0; c < comp.size(); c++) {
			static sf::Vector2f tempEnd;

			for (int cc = 0; cc < 2; cc++) {

				//tempEnd
				if (cc == 0) { /*Front*/ tempEnd.x = comp[c].x; tempEnd.y = CircuitGUI::comp[c].y; }
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

	bool Occupied = 0;
	bool occupiedAt(int Index, const sf::Vector2f& At, bool ignoreAllVir = false) {
		//if (ignoreAllVir) if (std::count(virSerial.begin(), virSerial.end(), c)) continue;
		static Entity indexEntity;
		static sf::Vector2f end;
		indexEntity = CircuitGUI::comp[Index];
		indexEntity.x = At.x; indexEntity.y = At.y;
		indexEntity.stimuli();
		end = indexEntity.getEndPos();

		int noCount = 0;
		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			//if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (c == Index
				|| ((comp[c].x - At.x) >= 200 || (At.x - comp[c].x) >= 200)
				|| ((comp[c].y - At.y) >= 200 || (At.y - comp[c].y) >= 200)
				) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(virSerial.begin(), virSerial.end(), c) != virSerial.end()) continue;

			if (At.x == CircuitGUI::comp[c].x && At.y == CircuitGUI::comp[c].y) {
				if (CircuitGUI::comp[Index].angle != CircuitGUI::comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (At.x == CircuitGUI::comp[c].getEndPos().x && At.y == CircuitGUI::comp[c].getEndPos().y) {
				if (abs((int)(CircuitGUI::comp[Index].angle - CircuitGUI::comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (end.x == CircuitGUI::comp[c].x && end.y == CircuitGUI::comp[c].y) {
				if (abs((int)(CircuitGUI::comp[Index].angle - CircuitGUI::comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (end.x == CircuitGUI::comp[c].getEndPos().x && end.y == CircuitGUI::comp[c].getEndPos().y) {
				if (CircuitGUI::comp[Index].angle != CircuitGUI::comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

		}
		if (!!noCount) { Occupied = 0; return 0; } // Non-Zero (!!bool)




		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			//if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (c == Index
				|| ((comp[c].x - At.x) >= 200 || (At.x - comp[c].x) >= 200)
				|| ((comp[c].y - At.y) >= 200 || (At.y - comp[c].y) >= 200)
				) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(virSerial.begin(), virSerial.end(), c) != virSerial.end()) continue;


			if (indexEntity.bounds.intersects(CircuitGUI::comp[c].bounds)) { Occupied = 1; return 1; }
		}

		Occupied = 0; return 0;
	}
	sf::FloatRect areaofCollection(bool collectionIsAllComp = false) {
		sf::FloatRect compBound;
		sf::FloatRect ABCD;

		if (collectionIsAllComp) {
			if (comp.size()) {
				compBound = comp.front().bounds;
				ABCD = compBound;
			}
			for (int c = 0; c < comp.size(); c++) {
				compBound = comp[c].bounds;
				if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
				if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
				if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
				if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
			}
		}
		else {
			if (virSerial.size()) {
				compBound = comp[virSerial.front()].bounds;
				ABCD = compBound;
			}
			for (int v = 0; v < virSerial.size(); v++) {
				compBound = comp[virSerial[v]].bounds;
				if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
				if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
				if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
				if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
			}
		}

		return ABCD;
	}


	void colorEntityboarder() {

		sf::Color temp;

		if (darkLightMode)
			temp = { 0, 255, 85 };
		else
			temp = { 0/*200*/, 204, 102 };

		Entity::setboarderDesgin(temp);

		for (int c = 0; c < comp.size(); c++)
			comp[c].boarder.setOutlineColor(temp);

	}
	sf::CircleShape nodePic(4, 15);
	//void nodePicDesign()
	sf::RectangleShape selSqr;
	void selSqrDesign() {
		selSqr.setOutlineThickness(1.0f);
		if (darkLightMode) {
			selSqr.setFillColor(sf::Color(0, 89, 179, 90));//128, 217, 38 | 38, 83, 217
			selSqr.setOutlineColor(sf::Color(0, 89, 179));
		}
		else {
			selSqr.setFillColor(sf::Color(66, 135, 245, 60));
			selSqr.setOutlineColor(sf::Color(66, 135, 245));
		}
	}
	void drawSelSqr() {
		app.draw(selSqr);
	}
	sf::RectangleShape allSqr;
	void allSqrDesign() {
		allSqr.setFillColor(sf::Color::Transparent);
		allSqr.setOutlineThickness(1.0f);

		if (darkLightMode) allSqr.setOutlineColor(sf::Color(0, 0, 0/*0, 51, 102*/));//0, 255, 85
		else allSqr.setOutlineColor(sf::Color(249, 140, 31));
	}
	void updateAllSqr() {

		sf::FloatRect virArea(areaofCollection(false));

		allSqr.setSize(sf::Vector2f(virArea.width, virArea.height));
		allSqr.setPosition(sf::Vector2f(virArea.left, virArea.top));

	}
	void drawAllSqr() {
		app.draw(allSqr);
	}


	/*ToolBox*/
	const float c_toolColWidth = 100;
	sf::Vector2f toolWinRestPos(0, 0);
	sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, CircuitGUI::view.getSize().y));
	sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(c_toolColWidth, c_toolColWidth));
	sf::Sprite ToolSpr[7]; //noOfComps-1
	sf::Vector2f ToolSprPOS[7]; //noOfComps-1
	//void toolBoxInit()
	void updatePosToolBox()
	{
		toolWinRestPos = onScreen(0, 0);
		for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setPosition(onScreen(ToolSprPOS[c].x, ToolSprPOS[c].y));
	}
	void updateEndCircles()
	{
		while (allEnds.size() < allEndCircles.size())
			allEndCircles.erase(allEndCircles.begin() + allEnds.size(), allEndCircles.end());


		while (allEndCircles.size() < allEnds.size())
			allEndCircles.emplace_back(nodePic);


		for (int e = 0; e < allEndCircles.size(); e++)
			allEndCircles[e].setPosition(allEnds[e]);

		//cout << "\n" << allEnds.size();
	}
	void drawToolColumn(bool MInTool, bool MIntool) {
		if (MInTool) {
			CircuitGUI::app.draw(CircuitGUI::toolCol);

			for (int c = 0; c < (Entity::noOfComps - 1); c++) { CircuitGUI::ToolSpr[c].setColor(sf::Color(255, 255, 255, CircuitGUI::ToolSpr[c].getColor().a + (255 - CircuitGUI::ToolSpr[c].getColor().a) / 15)); }
			for (int c = 0; c < (Entity::noOfComps - 1); c++) { CircuitGUI::app.draw(CircuitGUI::ToolSpr[c]); }
		}
		else { for (int c = 0; c < (Entity::noOfComps - 1); c++) CircuitGUI::ToolSpr[c].setColor(sf::Color::Transparent); }

		if (MIntool) CircuitGUI::app.draw(CircuitGUI::ToolBoxLittleBox);
	}


	struct quadTree {
		bool isSubDivided = false;
		unsigned int size = 0;
		int arr[5] = { 0,0,0,0,0 };
		sf::FloatRect area;
		sf::RectangleShape rectDraw;								// [0 1]
		quadTree* sub[4] = { nullptr,nullptr ,nullptr ,nullptr };	// [2 3]
	};
	quadTree qt;
	void qtDelete(quadTree& box) {
		if (box.isSubDivided) {
			qtDelete(*box.sub[0]);
			qtDelete(*box.sub[1]);
			qtDelete(*box.sub[2]);
			qtDelete(*box.sub[3]);

			delete box.sub[0];
			delete box.sub[1];
			delete box.sub[2];
			delete box.sub[3];

			box.sub[0] = nullptr;
			box.sub[1] = nullptr;
			box.sub[2] = nullptr;
			box.sub[3] = nullptr;

			box.size = 0;
			box.isSubDivided = false;
			box.area = { 0,0,0,0 };
			box.rectDraw.setSize(zero);
		}
	}
	void qtAdd(int c, quadTree& box) {
		if (box.area.intersects(comp[c].bounds)) {
			if (box.size < 5) { box.arr[box.size++] = c; } //HardCode
			else {
				if (!box.isSubDivided) {
					// Sub-divide
					box.isSubDivided = 1;
					int qtHalfWidth = (int)(box.area.width / 2), qtHalfHeight = (int)(box.area.height / 2);
					box.sub[0] = new quadTree; box.sub[0]->area.width = qtHalfWidth; box.sub[0]->area.height = qtHalfHeight; box.sub[0]->area.left = box.area.left;					box.sub[0]->area.top = box.area.top;
					box.sub[1] = new quadTree; box.sub[1]->area.width = qtHalfWidth; box.sub[1]->area.height = qtHalfHeight; box.sub[1]->area.left = box.area.left + qtHalfWidth;	box.sub[1]->area.top = box.area.top;
					box.sub[2] = new quadTree; box.sub[2]->area.width = qtHalfWidth; box.sub[2]->area.height = qtHalfHeight; box.sub[2]->area.left = box.area.left;					box.sub[2]->area.top = box.area.top + qtHalfHeight;
					box.sub[3] = new quadTree; box.sub[3]->area.width = qtHalfWidth; box.sub[3]->area.height = qtHalfHeight; box.sub[3]->area.left = box.area.left + qtHalfWidth;	box.sub[3]->area.top = box.area.top + qtHalfHeight;

					// Add qtArr in subdivided areas
					for (int i = 0; i < 5; i++) //HardCode
					{
						/*	   if (*/qtAdd(box.arr[i], *box.sub[0])/*)*/;
						/*else if (*/qtAdd(box.arr[i], *box.sub[1])/*)*/;
						/*else if (*/qtAdd(box.arr[i], *box.sub[2])/*)*/;
						/*else if (*/qtAdd(box.arr[i], *box.sub[3])/*)*/;
					}
				}

				/*	   if (*/qtAdd(c, *box.sub[0])/*)*/;
				/*else if (*/qtAdd(c, *box.sub[1])/*)*/;
				/*else if (*/qtAdd(c, *box.sub[2])/*)*/;
				/*else if (*/qtAdd(c, *box.sub[3])/*)*/;
				box.size++;
			}

			//return 1;
		}
		//else return 0;
	}
	void qtUpdate() {
		qtDelete(qt);
		int compSize = comp.size();
		qt.area = areaofCollection(true);
		if (compSize <= 5) { //HardCode
			qt.size = compSize;
			for (int c = 0; c < compSize; c++)
				qt.arr[c] = c;
		}
		else {
			for (int c = 0; c < compSize; c++) //HardCode
				qtAdd(c, qt);
		}
	}
	void qtWrite(quadTree& box = qt, int taaabbss = 0) {

		for (int t = 0; t < taaabbss; t++) std::cout << "\t";
		std::cout << "(" << box.size << "):" << box.arr[0] << ", " << box.arr[1] << ", " << box.arr[2] << ", " << box.arr[3] << ", " << box.arr[4] << "\n";

		if (box.isSubDivided) {
			qtWrite(*box.sub[0], taaabbss + 1);
			qtWrite(*box.sub[1], taaabbss + 1);
			qtWrite(*box.sub[2], taaabbss + 1);
			qtWrite(*box.sub[3], taaabbss + 1);
		}

	}
	void qtDraw(quadTree& box) {

		box.rectDraw.setSize(sf::Vector2f(box.area.width, box.area.height));
		box.rectDraw.setPosition(sf::Vector2f(box.area.left, box.area.top));
		box.rectDraw.setFillColor(sf::Color(box.rectDraw.getPosition().y, box.rectDraw.getPosition().x, box.rectDraw.getScale().y, 100));

		app.draw(box.rectDraw);

		if (box.isSubDivided) {
			qtDraw(*box.sub[0]);
			qtDraw(*box.sub[1]);
			qtDraw(*box.sub[2]);
			qtDraw(*box.sub[3]);
		}

	}
	void qtExtract(sf::FloatRect rect, std::vector<int>& vec) { ; }


	void updateThemeColors()
	{
		if (darkLightMode) tempDimColor = { 60, 60, 60 };
		else tempDimColor = { 150, 150, 150 };

		colorGrid();
		colorBackground();
		colorBrightLineGrid();
		selSqrDesign();
		allSqrDesign();
		colorEntityboarder();
	}
	void initializeGUI()
	{
		updateThemeColors();

		//setFont
		{
			//ResourceID_Fonts.SetAll(IDR_FONT1, "FONT");
			//Entity::setFont(ResourceID_Fonts.GetResource().ptr, ResourceID_Fonts.GetResource().size_bytes);
			Entity::setFont("assets/Fonts/CalibriL_1.ttf");
		}

		//renderWinInit();
		{
			W = sf::VideoMode::getDesktopMode().width * 0.85;
			H = W * 9 / 16;
			if (H < c_toolColWidth * 7) H = c_toolColWidth * 7;

			app.create(sf::VideoMode((unsigned int)W, (unsigned int)H), "CircuitSim", sf::Style::Default, sf::ContextSettings(0, 0, 8));
			W = app.getSize().x; H = app.getSize().y;
			
			app.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - W / 2, sf::VideoMode::getDesktopMode().height / 2 - H / 2 - 50));
			app.setFramerateLimit(60);
			app.setVerticalSyncEnabled(1);
			app.setKeyRepeatEnabled(false);
		}

		//viewInit();
		{
			view.setCenter(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2));
			view.setSize((sf::Vector2f)app.getSize());
			//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), sf::Vector2f(app.getSize().x * 2, app.getSize().y * 2));
			//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), (sf::Vector2f)app.getSize());
		}

		//setIcon
		{
			Resource_Logo.SetAll(IDR_LOGO, "BMP");

			if (Resource_Logo.GetResource().ptr != nullptr) {
				sf::Image logo; // No need to keep it alive after setIcon()
				if (logo.loadFromMemory(Resource_Logo.GetResource().ptr, Resource_Logo.GetResource().size_bytes))
					app.setIcon(logo.getSize().x, logo.getSize().y, logo.getPixelsPtr());
			}
		}

		//loadTextures();
		{
			Resource_Images.resize(8);

			Resource_Images[Entity::Cap].SetAll(IDR_CAP, "BMP");
			Resource_Images[Entity::Cur].SetAll(IDR_CUR, "BMP");
			Resource_Images[Entity::Dod].SetAll(IDR_DOD, "BMP");
			Resource_Images[Entity::Ind].SetAll(IDR_IND, "BMP");
			Resource_Images[Entity::Res].SetAll(IDR_RES, "BMP");
			Resource_Images[Entity::SwO].SetAll(IDR_SWO, "BMP");
			Resource_Images[Entity::Vol].SetAll(IDR_VOL, "BMP");
			Resource_Images[Entity::SwC].SetAll(IDR_SWC, "BMP");

			compTex.resize(8);
			compTex[Entity::Cap].loadFromMemory(Resource_Images[Entity::Cap].GetResource().ptr, Resource_Images[Entity::Cap].GetResource().size_bytes);
			compTex[Entity::Cur].loadFromMemory(Resource_Images[Entity::Cur].GetResource().ptr, Resource_Images[Entity::Cur].GetResource().size_bytes);
			compTex[Entity::Dod].loadFromMemory(Resource_Images[Entity::Dod].GetResource().ptr, Resource_Images[Entity::Dod].GetResource().size_bytes);
			compTex[Entity::Ind].loadFromMemory(Resource_Images[Entity::Ind].GetResource().ptr, Resource_Images[Entity::Ind].GetResource().size_bytes);
			compTex[Entity::Res].loadFromMemory(Resource_Images[Entity::Res].GetResource().ptr, Resource_Images[Entity::Res].GetResource().size_bytes);
			compTex[Entity::SwO].loadFromMemory(Resource_Images[Entity::SwO].GetResource().ptr, Resource_Images[Entity::SwO].GetResource().size_bytes);
			compTex[Entity::Vol].loadFromMemory(Resource_Images[Entity::Vol].GetResource().ptr, Resource_Images[Entity::Vol].GetResource().size_bytes);
			compTex[Entity::SwC].loadFromMemory(Resource_Images[Entity::SwC].GetResource().ptr, Resource_Images[Entity::SwC].GetResource().size_bytes);
		}

		//initializeGrid();
		{
			colorGrid();
			colorBackground();

			int a = (int)((sf::VideoMode::getDesktopMode().width - W) / 2);
			int b = (int)((sf::VideoMode::getDesktopMode().height - H) / 2);
			virtualBoarder = (a >= b) ? a + 5 * gap : b + 5 * gap;

			vLines.reserve((W + 2 * virtualBoarder) / gap + 2);
			sf::Vector2f tempVect(1, H + 2 * virtualBoarder);
			for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
				vLines.emplace_back(tempVect);
			}
			for (int c = 0; c < vLines.size(); c++) {
				vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//vLines[c].setFillColor(gridColor);
			}

			hLines.reserve((H + 2 * virtualBoarder) / gap + 2);
			tempVect.x = W + 2 * virtualBoarder; tempVect.y = 1;
			for (int c = 0; c <= (H + 2 * virtualBoarder) / gap; c++) {
				hLines.emplace_back(tempVect);
			}
			for (int c = 0; c < hLines.size(); c++) {
				hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//hLines[c].setFillColor(gridColor);
			}

			colorBrightLineGrid();
		}

		/* {
			colorGrid();
			colorBackground();

			vLines.reserve((W + 2 * virtualBoarder) / gap + 2);
			sf::Vector2f tempVect(1, H + 2 * virtualBoarder);
			for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
				vLines.emplace_back(tempVect);
			}
			for (int c = 0; c < vLines.size(); c++) {
				vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//vLines[c].setFillColor(gridColor);
			}

			hLines.reserve((H + 2 * virtualBoarder) / gap + 2);
			tempVect.x = W + 2 * virtualBoarder; tempVect.y = 1;
			for (int c = 0; c <= (H + 2 * virtualBoarder) / gap; c++) {
				hLines.emplace_back(tempVect);
			}
			for (int c = 0; c < hLines.size(); c++) {
				hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//hLines[c].setFillColor(gridColor);
			}

			colorBrightLineGrid();
		}*/

		//toolBoxInit();
		{
			/*Color*/
			CircuitGUI::toolCol.setFillColor(sf::Color(0, 0, 0, 120));
			CircuitGUI::ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));

			/*Sprites*/
			for (int c = 0; c < (Entity::noOfComps - 1); c++) {
				ToolSpr[c].setTexture(compTex[c]);
				ToolSpr[c].setOrigin(compTex[c].getSize().x / 2, 0);
				//compSpr[c].setRotation(45);
				ToolSprPOS[c].x = CircuitGUI::c_toolColWidth / 2;
				ToolSprPOS[c].y = c * CircuitGUI::c_toolColWidth + (int)(CircuitGUI::c_toolColWidth - compTex[c].getSize().y) / 2;
				ToolSpr[c].setPosition(CircuitGUI::view.getCenter().x - CircuitGUI::view.getSize().x / 2 + ToolSprPOS[c].x, CircuitGUI::view.getCenter().y - CircuitGUI::view.getSize().y / 2 + ToolSprPOS[c].y);
			}
		}

		//void nodePicDesign();
		{
			nodePic.setOrigin(4, 4);
			nodePic.setFillColor(CircuitGUI::normalCompColor);
		}

		selSqrDesign();

		allSqrDesign();


		/*Vectors*/
		comp.reserve(3);
		allEnds.reserve(3);
		virSerial.reserve(3);
		virSprite.reserve(3);
		virSerialShift.reserve(3);
		allEndCircles.reserve(17);
		//for (int c = 0; c < 16; c++) comp.emplace_back(&compTex[c % 8], c * 90 + 200, c * 90 + 100, c * 90);
	}


	namespace Options
	{
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
			//}m

			sf::Image OutputImage;
			sf::FloatRect compBound;
			sf::FloatRect ABCD;

			if (comp.size() != 0)
			{
				ABCD = areaofCollection(true);
				OutputImage.create(abs(ABCD.width) + 30, abs(ABCD.height) + 30);
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

			WCHAR hello[260] = { 0 };
			for (int i = 0; i < 260 && i < filepath.length(); i++) hello[i] = filepath[i];
			ShellExecute(NULL, NULL, hello, NULL, NULL, SW_SHOWNORMAL);

		}

		void openf(const std::string& filepath) {

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
			virSerial.clear();
			virSprite.clear();
			virSerialShift.clear();
			virSerial.reserve(comp.size());
			virSprite.reserve(comp.size());

			// Centering
			int no = 0;
			input >> no;
			comp.reserve(no + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
				input >> S >> X >> Y >> A;
				comp.emplace_back(S % (Entity::noOfComps + 1), trim(X), trim(Y), ((A % 360) / 90) * 90);
			}
			input.close();

			// Centering
			sf::FloatRect virArea = areaofCollection(true);
			sf::Vector2f offSet(view.getCenter().x - (virArea.left + (int)(virArea.width / 2)), view.getCenter().y - (virArea.top + (int)(virArea.height / 2)));
			offSet = trim(offSet);

			for (int c = 0; c < comp.size(); c++)
			{
				comp[c].x = (int)trim(comp[c].x + offSet.x);
				comp[c].y = (int)trim(comp[c].y + offSet.y);
				comp[c].stimuli();
			}
		}

		void savef(const std::string& file) {

			//std::cout << "\nCtrl + S\n";
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

			WCHAR hello[260] = { 0 };
			for (int i = 0; i < 260 && i < file.length(); i++) hello[i] = file[i];
			ShellExecute(NULL, NULL, hello, NULL, NULL, SW_SHOWNORMAL);
		}

		void copyf() {

			std::ofstream output;
			std::string tempStr(std::to_string((int)virSerial.size()) + "\n");

			int size = (int)virSerial.size();
			for (int c = 0; c < size; c++) {
				tempStr += std::to_string(comp[virSerial[c]].serial) + "\t" + std::to_string((int)comp[virSerial[c]].x) + "\t" + std::to_string((int)comp[virSerial[c]].y) + "\t" + std::to_string((int)comp[virSerial[c]].angle) + "\n";
			}

			//clipboard << tempStr;
			sf::Clipboard::setString(tempStr);

		}

		void pastef() {

			std::string inString(sf::Clipboard::getString());
			std::vector<int> integers; integers.reserve(21);

			bool negative = 0, numStarted = 0;
			for (int c = 0, x = 0, temp = 0; c < inString.size(); c++) {
				temp = (int)inString[c];

				if ('0' <= temp && temp <= '9') {
					numStarted = 1;
					x *= 10;
					x += temp - (int)'0';
				}
				else {
					if (temp == '-') negative = 1;
					else if (numStarted) {
						if (negative) x *= -1;
						integers.emplace_back(x);
						negative = 0;
						x = 0;
						numStarted = 0;
					}
				}
			}

			int OffsetX = 0, OffsetY = 0, count = 0;
			for (int i = 1; i + 2 < integers.size(); i += 4, count++)
			{
				OffsetX += integers[i + 1];
				OffsetY += integers[i + 2];
			}
			if (count) {
				OffsetX /= count;
				OffsetY /= count;

				OffsetX = view.getCenter().x - OffsetX;
				OffsetY = view.getCenter().y - OffsetY;
			}

			int noOfComponentsBefore = comp.size();
			comp.reserve(abs(integers[0]) + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; 1 + c + 4 <= integers.size();) {
				S = abs(integers[++c]);
				X = integers[++c] + OffsetX;
				Y = integers[++c] + OffsetY;
				A = abs(integers[++c]);
				comp.emplace_back(S % (Entity::noOfComps + 1), trim(X), trim(Y), ((A % 360) / 90) * 90);
			}

			virSerial.clear();
			virSprite.clear();
			virSerialShift.clear();

			virSerial.reserve(comp.size() - noOfComponentsBefore);
			virSprite.reserve(comp.size() - noOfComponentsBefore);

			for (int vv = 0; vv < (comp.size() - noOfComponentsBefore); vv++) {
				virSerial.emplace_back(vv + noOfComponentsBefore);
				virSprite.emplace_back(comp[virSerial.back()].sprite);
				virSprite.back().setColor(tempDimColor);
			}
		}

		void rotatef() {

			for (int v = 0; v < virSerial.size(); v++) {
				comp[virSerial[v]].angle += 90;
				comp[virSerial[v]].angle -= (int)(comp[virSerial[v]].angle / 360) * 360;
				comp[virSerial[v]].stimuli();
			}

		}

		void deletef() {

			//erase - remove idiom
			auto iter = std::remove_if(comp.begin(), comp.end(), [&](const auto& elem) {
				return std::binary_search(virSerial.begin(), virSerial.end(), &elem - &comp[0]);
				});
			comp.erase(iter, comp.end());

			virSerial.clear();
			virSprite.clear();
			virSerialShift.clear();

			/*

			// As complexity of eraseing one element is same as eraseing 100's(more than one) of elements
			//   | e.g.
			//   |--> complexity of erase(v.begin() + i) IS EQUAL TO complexity of erase(v.begin() + i, v.begin() + 1000)

			// So we will erase all the elements in a sorted set together
			//   | e.g.
			//   |--> in [1,2,3,4,7,8,9,10,11,51], we will get three sets [1,2,3,4], [7,8,9,10,11] and [51]. And each set will be erased at a time.
			//   |--> so instead of erasing 10 elements sepreatly we erase three sets (saving us time of erasing 7(10-3) elements).

			int totalKills = 0;
			while (!!virSerial.size()) {
				int v = 1;
				for (; v < virSerial.size(); v++)
					if (virSerial[v - 1] + 1 < virSerial[v]) break;

				comp.erase(comp.begin() + (virSerial[0] - totalKills), comp.begin() + (virSerial[0] - totalKills + v));
				virSerial.erase(virSerial.begin(), virSerial.begin() + v);

				totalKills += v;
			}

			virSerial.clear();
			virSprite.clear();
			virSerialShift.clear();
			*/

			/*while (0 < virSerial.size()) {
				if (comp.size() > 0) comp.erase(comp.begin() + virSerial[0]);
				if (virSprite.size() > 0) virSprite.erase(virSprite.begin());

				for (int c = 1; c < virSerial.size(); c++) {
					if (virSerial[c] > virSerial[0]) virSerial[c]--;
				}

				virSerial.erase(virSerial.begin());
			}*/

			/* {
				for (int v = 0; v < virSerial.size(); v++)
					comp.erase(comp.begin() + (virSerial[v] - v)); //assuming vir or virSerial is sorted(ascendingly)
			}*/

			/*int total = 0;
			while (!!virSerial.size())
			{
				int v = 1;
				for (; v < virSerial.size(); v++)
					if (virSerial[v - 1] + 1 < virSerial[v]) break;


				comp.erase(comp.begin() + virSerial[0] - total, comp.begin() + virSerial[0] - total + v);
				virSerial.erase(virSerial.begin(), virSerial.begin() + v);

				total += v;
			}*/
		}
	}

	/*namespace Response {
		enum State {
			None				= 0,
			Mouse_Drag			= 1,
			Mouse_Left_Down		= 2,
			Mouse_In_ToolBar	= 4,
			Mouse_Selection		= 8,
			Mouse_Hold_Item		= 16,
			Mouse_In_MenuBar	= 32,
			Keyboard			= 64
		};
	}*/

	/* {
			using namespace CircuitGUI::Response;

			if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left)
				myWinState |= State::Mouse_Left_Down;

			if (evnt.type == evnt.KeyPressed)
				myWinState |= State::Keyboard;

			if (cursorInWin && sf::Mouse::getPosition(app).x <= CircuitGUI::c_toolColWidth)
				myWinState |= State::Mouse_In_ToolBar;

			if (cursorInWin && sf::Mouse::getPosition(app).y <= 17)
				myWinState |= State::Mouse_In_MenuBar;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && !(myWinState & State::Mouse_In_ToolBar))
				myWinState |= State::Mouse_Drag;

			if ((myWinState & State::Mouse_Left_Down) && !(myWinState & State::Mouse_In_ToolBar)) {
				static int x = 0;
				cout << "\nYaeh " << x++;
			}
			//if ((myWinState & State::Mouse_Left_Down) && !(myWinState & State::Mouse_In_ToolBar)) cout << "\txor "<< myWinState;


		}*/
}

/*

bool occupiedAt(int Index, const sf::Vector2f& At, bool ignoreAllVir = false) {
		//if (ignoreAllVir) if (std::count(virSerial.begin(), virSerial.end(), c)) continue;
		int noCount = 0;
		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			if (c == Index || abs(CircuitGUI::comp[c].x - At.x) >= 200 || abs(CircuitGUI::comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(virSerial.begin(), virSerial.end(), c) != virSerial.end()) continue;

			if (At.x == CircuitGUI::comp[c].x && At.y == CircuitGUI::comp[c].y) {
				if (CircuitGUI::comp[Index].angle != CircuitGUI::comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (At.x == CircuitGUI::comp[c].getEndPos().x && At.y == CircuitGUI::comp[c].getEndPos().y) {
				if (abs((int)(CircuitGUI::comp[Index].angle - CircuitGUI::comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

		}
		if (!!noCount) { Occupied = 0; return 0; } //if (noCount == 1) { Occupied = 0; return 0; }


		static Entity indexEntity;
		indexEntity = CircuitGUI::comp[Index];
		indexEntity.x = At.x; indexEntity.y = At.y;
		indexEntity.stimuli();
		static sf::Vector2f end;
		end = indexEntity.getEndPos();

		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			if (c == Index || abs(CircuitGUI::comp[c].x - end.x) >= 200 || abs(CircuitGUI::comp[c].y - end.y) >= 200) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(virSerial.begin(), virSerial.end(), c) != virSerial.end()) continue;

			if (end.x == CircuitGUI::comp[c].x && end.y == CircuitGUI::comp[c].y) {
				if (abs((int)(CircuitGUI::comp[Index].angle - CircuitGUI::comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (end.x == CircuitGUI::comp[c].getEndPos().x && end.y == CircuitGUI::comp[c].getEndPos().y) {
				if (CircuitGUI::comp[Index].angle != CircuitGUI::comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

		}
		if (!!noCount) { Occupied = 0; return 0; } //if (noCount == 1) { Occupied = 0; return 0; }

		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			if (c == Index || abs(CircuitGUI::comp[c].x - At.x) >= 200 || abs(CircuitGUI::comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(virSerial.begin(), virSerial.end(), c) != virSerial.end()) continue;

			if (indexEntity.bounds.intersects(CircuitGUI::comp[c].bounds)) { Occupied = 1; return 1; }

		}

		Occupied = 0; return 0;
	}

*/