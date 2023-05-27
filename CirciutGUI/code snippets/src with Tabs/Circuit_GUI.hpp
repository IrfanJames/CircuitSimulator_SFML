#pragma once

#include <windows.h> //To Open file(txt/PNG) just after creating(Save as...) it

#include "SFML/Graphics.hpp"
#include "Circuit_Scene.hpp"

extern sf::Texture compTex[8];

namespace CircuitGUI {

	/*Constants*/
	const int gap = 15;
	const sf::Vector2f zero(0, 0);
	const sf::Color normalCompColor(230, 230, 230);

	bool darkLightMode = false;
	sf::Color tempDimColor(60, 60, 60/*150, 150, 150*/);

	/*Textures*/
	//void loadTextures()


	sf::RenderWindow app;
	//renderWinInit();
	//sf::View view; //xzx
	//viewInit();


	/*Vectors*/ //xzx
	/*std::vector<Entity> comp;
	std::vector<int> virSerial;
	std::vector<sf::Sprite> virSprite;
	std::vector<int> virSerialShift;
	std::vector<sf::CircleShape> allEndCircles;
	std::vector<sf::Vector2f> allEnds;*/
	int csi = 0; // Short for CurrentSceneIndex
	std::vector<Scene> scenes;
	void drawComp() {
		for (int c = 0; c < scenes[csi].comp.size(); c++) { scenes[csi].comp[c].draw(app); }
	}
	void drawVirSprites() {
		for (int v = 0; v < scenes[csi].virSprite.size(); v++)
			app.draw(scenes[csi].virSprite[v]);
	}
	void drawNodes() {
		for (int e = 0; e < scenes[csi].allEndCircles.size(); e++) { app.draw(scenes[csi].allEndCircles[e]); }
	}
	void drawBoarders() {
		for (int v = 0; v < scenes[csi].virSerial.size(); v++)
			app.draw(scenes[csi].comp[scenes[csi].virSerial[v]].boarder);
	}
	void updateAllEnds() {

		scenes[csi].allEnds.clear();

		int nodeCount = 0;
		for (int c = 0; c < scenes[csi].comp.size(); c++) {
			static sf::Vector2f tempEnd;

			for (int cc = 0; cc < 2; cc++) {

				//tempEnd
				if (cc == 0) { /*Front*/ tempEnd.x = scenes[csi].comp[c].x; tempEnd.y = scenes[csi].comp[c].y; }
				if (cc == 1) { /*Rear*/  tempEnd = scenes[csi].comp[c].getEndPos(); }

				bool found = 0;
				for (int e = 0; e < scenes[csi].allEnds.size(); e++) {
					if (tempEnd == scenes[csi].allEnds[e]) {
						found = 1;
						break;
					}
				}

				if (!found) {
					nodeCount++;

					if (nodeCount <= scenes[csi].allEnds.size()) scenes[csi].allEnds[nodeCount] = tempEnd;
					else scenes[csi].allEnds.emplace_back(tempEnd);
				}

			}
		}

		while (nodeCount < scenes[csi].allEnds.size())
			scenes[csi].allEnds.pop_back();

	}


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
		return sf::Vector2f(scenes[csi].view.getCenter().x - scenes[csi].view.getSize().x / 2 + X, scenes[csi].view.getCenter().y - scenes[csi].view.getSize().y / 2 + Y);
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
	//std::vector<sf::RectangleShape> vLines; //xzx
	//std::vector<sf::RectangleShape> hLines; //xzx
	int virtualBoarder = 80;
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
		for (int c = 0; c < scenes[csi].vLines.size(); c++) { app.draw(scenes[csi].vLines[c]); }
		for (int c = 0; c < scenes[csi].hLines.size(); c++) { app.draw(scenes[csi].hLines[c]); }
	}
	//void initializeGrid()


	/*Drag*/ // xzx
	bool dragBool = 0;
	/*float viewX = 0, viewY = 0;
	float verX = 0, verY = 0;
	float horX = 0, horY = 0;
	int verBrightCount = 5, horBrightCount = 5;*/
	void initializeDrag() {
		mouseHoldX = (float)sf::Mouse::getPosition(app).x; mouseHoldY = (float)sf::Mouse::getPosition(app).y;
		scenes[csi].viewX = scenes[csi].view.getCenter().x, scenes[csi].viewY = scenes[csi].view.getCenter().y;
		scenes[csi].verX = scenes[csi].vLines[0].getPosition().x; scenes[csi].verY = scenes[csi].vLines[0].getPosition().y;
		scenes[csi].horX = scenes[csi].hLines[0].getPosition().x; scenes[csi].horY = scenes[csi].hLines[0].getPosition().y;
	}
	void Drag() {
		scenes[csi].view.setCenter(sf::Vector2f(scenes[csi].viewX + mouseHoldX - (float)sf::Mouse::getPosition(app).x, scenes[csi].viewY + mouseHoldY - (float)sf::Mouse::getPosition(app).y));
		float newVerY = scenes[csi].verY + mouseHoldY - (float)sf::Mouse::getPosition(app).y;
		float newHorX = scenes[csi].horX + mouseHoldX - (float)sf::Mouse::getPosition(app).x;


		float verBrightX = scenes[csi].vLines[scenes[csi].verBrightCount].getPosition().x;
		for (int c = 0; c < scenes[csi].vLines.size(); c++) {
			scenes[csi].vLines[c].setPosition(trim(newHorX) + c * gap, newVerY);
		}
		float horBrightY = scenes[csi].hLines[scenes[csi].horBrightCount].getPosition().y;
		for (int c = 0; c < scenes[csi].hLines.size(); c++) {
			scenes[csi].hLines[c].setPosition(newHorX, trim(newVerY) + c * gap);
		}

		verBrightX -= scenes[csi].vLines[scenes[csi].verBrightCount].getPosition().x;
		if (verBrightX > gap * 0.9) scenes[csi].verBrightCount--; else if (verBrightX < -gap * 0.9) scenes[csi].verBrightCount++;
		//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + (int)horX % gap) / gap;
		//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + (-virtualBoarder - trim(horX,gap))) / gap;
		//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) + horX - (horX / (float)gap)*gap) / gap;
		//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x + (-virtualBoarder + viewX - W / 2) - verX) / gap);
		//verBrightCount = wxyz + (int)((mouseX - (float)Mouse::getPosition(app).x) - (int)horX / gap) / gap;
		//cout << "\n" << mouseX - (float)Mouse::getPosition(app).x << ", " << + (-virtualBoarder + viewX - W / 2) - verX << ", " << verBrightCount;

		scenes[csi].verBrightCount = abs((scenes[csi].verBrightCount < 1) * (5 + scenes[csi].verBrightCount % 5) + (1 <= scenes[csi].verBrightCount) * ((scenes[csi].verBrightCount - 1) % 5 + 1)) % 6;
		//cout << "\n" << verBrightCount << "\n";


		horBrightY -= scenes[csi].hLines[scenes[csi].horBrightCount].getPosition().y;
		if (horBrightY > gap * 0.9) scenes[csi].horBrightCount--; else if (horBrightY < -gap * 0.9) scenes[csi].horBrightCount++;
		scenes[csi].horBrightCount = abs((scenes[csi].horBrightCount < 1) * (5 + scenes[csi].horBrightCount % 5) + (1 <= scenes[csi].horBrightCount) * ((scenes[csi].horBrightCount - 1) % 5 + 1)) % 6;
		//cout << "\n" << horBrightCount << "\n";
	}
	void colorBrightLineGrid(int index = csi) {
		for (int c = 0; c < scenes[index].vLines.size(); c++) {
			gridColor.a = 20 + ((c + scenes[index].verBrightCount) % 5 == 0) * 15;
			scenes[index].vLines[c].setFillColor(gridColor);
		}
		for (int c = 0; c < scenes[index].hLines.size(); c++) {
			gridColor.a = 20 + ((c + scenes[index].horBrightCount) % 5 == 0) * 15;
			scenes[index].hLines[c].setFillColor(gridColor);
		}
	}

	bool Occupied = 0;
	bool occupiedAt(int Index, const sf::Vector2f& At, bool ignoreAllVir = false) {
		//if (ignoreAllVir) if (std::count(virSerial.begin(), virSerial.end(), c)) continue;
		static Entity indexEntity;
		static sf::Vector2f end;
		indexEntity = scenes[csi].comp[Index];
		indexEntity.x = At.x; indexEntity.y = At.y;
		indexEntity.stimuli();
		end = indexEntity.getEndPos();

		int noCount = 0;
		for (int c = 0; c < scenes[csi].comp.size(); c++) {
			//if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (c == Index
				|| ((scenes[csi].comp[c].x - At.x) >= 200 || (At.x - scenes[csi].comp[c].x) >= 200)
				|| ((scenes[csi].comp[c].y - At.y) >= 200 || (At.y - scenes[csi].comp[c].y) >= 200)
				) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(scenes[csi].virSerial.begin(), scenes[csi].virSerial.end(), c) != scenes[csi].virSerial.end()) continue;

			if (At.x == scenes[csi].comp[c].x && At.y == scenes[csi].comp[c].y) {
				if (scenes[csi].comp[Index].angle != scenes[csi].comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (At.x == scenes[csi].comp[c].getEndPos().x && At.y == scenes[csi].comp[c].getEndPos().y) {
				if (abs((int)(scenes[csi].comp[Index].angle - scenes[csi].comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (end.x == scenes[csi].comp[c].x && end.y == scenes[csi].comp[c].y) {
				if (abs((int)(scenes[csi].comp[Index].angle - scenes[csi].comp[c].angle)) != 180) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

			if (end.x == scenes[csi].comp[c].getEndPos().x && end.y == scenes[csi].comp[c].getEndPos().y) {
				if (scenes[csi].comp[Index].angle != scenes[csi].comp[c].angle) {
					noCount++;
				}
				else {
					Occupied = 1; return 1;
				}
			}

		}
		if (!!noCount) { Occupied = 0; return 0; } // Non-Zero (!!bool)




		for (int c = 0; c < scenes[csi].comp.size(); c++) {
			//if (c == Index || abs(comp[c].x - At.x) >= 200 || abs(comp[c].y - At.y) >= 200) continue; // gap-hardcode
			if (c == Index
				|| ((scenes[csi].comp[c].x - At.x) >= 200 || (At.x - scenes[csi].comp[c].x) >= 200)
				|| ((scenes[csi].comp[c].y - At.y) >= 200 || (At.y - scenes[csi].comp[c].y) >= 200)
				) continue; // gap-hardcode
			if (ignoreAllVir) if (std::find(scenes[csi].virSerial.begin(), scenes[csi].virSerial.end(), c) != scenes[csi].virSerial.end()) continue;


			if (indexEntity.bounds.intersects(scenes[csi].comp[c].bounds)) { Occupied = 1; return 1; }
		}

		Occupied = 0; return 0;
	}
	sf::FloatRect areaofCollection(bool collectionIsAllComp = false) {
		sf::FloatRect compBound;
		sf::FloatRect ABCD;

		if (collectionIsAllComp) {
			if (scenes[csi].comp.size()) {
				compBound = scenes[csi].comp.front().bounds;
				ABCD = compBound;
			}
			for (int c = 0; c < scenes[csi].comp.size(); c++) {
				compBound = scenes[csi].comp[c].bounds;
				if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
				if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
				if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
				if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
			}
		}
		else {
			if (scenes[csi].virSerial.size()) {
				compBound = scenes[csi].comp[scenes[csi].virSerial.front()].bounds;
				ABCD = compBound;
			}
			for (int v = 0; v < scenes[csi].virSerial.size(); v++) {
				compBound = scenes[csi].comp[scenes[csi].virSerial[v]].bounds;
				if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
				if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
				if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
				if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
			}
		}

		return ABCD;
	}


	sf::CircleShape nodePic(4, 15);
	sf::RectangleShape selSqr;
	//sf::RectangleShape allSqr; //xzx
	void colorEntityboarder(int index = csi) {

		sf::Color temp;

		if (darkLightMode)
			temp = { 0, 255, 85 };
		else
			temp = { 0/*200*/, 204, 102 };

		Entity::setboarderDesgin(temp);

		for (int c = 0; c < scenes[index].comp.size(); c++)
			scenes[index].comp[c].boarder.setOutlineColor(temp);

	}
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
	void allSqrDesign(int index = csi) {
		scenes[index].allSqr.setFillColor(sf::Color::Transparent);
		scenes[index].allSqr.setOutlineThickness(1.0f);

		if (darkLightMode) scenes[index].allSqr.setOutlineColor(sf::Color(0, 0, 0/*0, 51, 102*/));//0, 255, 85
		else scenes[index].allSqr.setOutlineColor(sf::Color(249, 140, 31));
	}
	void updateAllSqr() {

		sf::FloatRect virArea(areaofCollection(false));

		scenes[csi].allSqr.setSize(sf::Vector2f(virArea.width, virArea.height));
		scenes[csi].allSqr.setPosition(sf::Vector2f(virArea.left, virArea.top));

	}
	void drawAllSqr() {
		app.draw(scenes[csi].allSqr);
	}
	//void nodePicDesign()


	/*ToolBox*/
	const float c_toolColWidth = 100;
	sf::Vector2f toolWinRestPos(0, 0);
	sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, 500/*scenes[csi].view.getSize().y*/)); //HardCode
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
		while (scenes[csi].allEnds.size() < scenes[csi].allEndCircles.size())
			scenes[csi].allEndCircles.erase(scenes[csi].allEndCircles.begin() + scenes[csi].allEnds.size(), scenes[csi].allEndCircles.end());


		while (scenes[csi].allEndCircles.size() < scenes[csi].allEnds.size())
			scenes[csi].allEndCircles.emplace_back(nodePic);


		for (int e = 0; e < scenes[csi].allEndCircles.size(); e++)
			scenes[csi].allEndCircles[e].setPosition(scenes[csi].allEnds[e]);

		//cout << "\n" << allEnds.size();
	}
	void drawToolColumn(bool MInTool, bool MIntool) {
		if (MInTool) {
			app.draw(toolCol);

			for (int c = 0; c < (Entity::noOfComps - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
			for (int c = 0; c < (Entity::noOfComps - 1); c++) { app.draw(ToolSpr[c]); }
		}
		else { for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setColor(sf::Color::Transparent); }

		if (MIntool) app.draw(ToolBoxLittleBox);
	}


	struct quadTree {
		bool isSubDivided = false;
		unsigned int size = 0;
		int arr[5] = { 0,0,0,0,0 };
		sf::FloatRect area;	// [0 1]
		sf::RectangleShape rectDraw;
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

		if (box.area.intersects(scenes[csi].comp[c].bounds)) {

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
		int compSize = scenes[csi].comp.size();
		qt.area = areaofCollection(true);
		if (compSize <= 5) { //HardCode
			qt.size = compSize;
			for (int c = 0; c < compSize; c++) qt.arr[c] = c;
		}
		else
			for (int c = 0; c < compSize; c++) //HardCode
				qtAdd(c, qt);
	}
	void qtWrite(quadTree& box = qt, int taaabbss = 0) {

		for (int t = 0; t < taaabbss; t++) std::cout << "\t";
		std::cout << "(" << box.size << "):" << box.arr[0] << ", " << box.arr[1] << ", " << box.arr[2] << ", " << box.arr[3] << ", " << box.arr[4] << "\n";
		//std::cout << box.size << "\n";

		if (box.isSubDivided) {
			qtWrite(*box.sub[0], taaabbss + 1);
			qtWrite(*box.sub[1], taaabbss + 1);
			qtWrite(*box.sub[2], taaabbss + 1);
			qtWrite(*box.sub[3], taaabbss + 1);
		}

	}
	void qtDraw(quadTree& box) {
		//std::cout << "ha ";
		box.rectDraw.setSize(sf::Vector2f(box.area.width, box.area.height));
		box.rectDraw.setPosition(sf::Vector2f(box.area.left, box.area.top));
		box.rectDraw.setFillColor(sf::Color(box.rectDraw.getPosition().y, box.rectDraw.getPosition().x, box.rectDraw.getScale().y, 100));
		//box.rectDraw.setOutlineColor(sf::Color(box.rectDraw.getScale().x, box.rectDraw.getPosition().y, box.rectDraw.getPosition().x, 100));
		//box.rectDraw.setFillColor(sf::Color(255, 0, 0, 50));
		//box.rectDraw.setOutlineThickness(1);
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

		for (int s = 0; s < scenes.size(); s++) {
			colorGrid();
			colorBackground();
			colorBrightLineGrid(s);
			selSqrDesign();
			allSqrDesign(s);
			colorEntityboarder(s);
		}
	}
	void initializeScene(int index = csi)
	{
		updateThemeColors();

		//viewInit();
		{
			scenes[index].view = sf::View(sf::FloatRect(0, 0, (int)app.getSize().x, (int)app.getSize().y));

			//scenes[index].view.setCenter(sf::Vector2f((int)(app.getSize().x / 2), (int)(app.getSize().y / 2)));
			//scenes[index].view.setSize((sf::Vector2f)app.getSize());
			//scenes[index].view.setSize(sf::Vector2f((int)app.getSize().x, (int)app.getSize().y));
			//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), sf::Vector2f(app.getSize().x * 2, app.getSize().y * 2));
			//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), (sf::Vector2f)app.getSize());
		}

		//initializeGrid();
		{
			colorGrid();
			colorBackground();

			int a = (int)((sf::VideoMode::getDesktopMode().width - W) / 2);
			int b = (int)((sf::VideoMode::getDesktopMode().height - H) / 2);
			virtualBoarder = (a >= b) ? a + 5 * gap : b + 5 * gap;

			scenes[csi].vLines.reserve((W + 2 * virtualBoarder) / gap + 2);
			sf::Vector2f tempVect(1, H + 2 * virtualBoarder);
			for (int c = 0; c <= (W + 2 * virtualBoarder) / gap; c++) {
				scenes[csi].vLines.emplace_back(tempVect);
			}
			for (int c = 0; c < scenes[csi].vLines.size(); c++) {
				scenes[csi].vLines[c].setPosition(-virtualBoarder - (int)(-virtualBoarder) % gap + c * gap, -virtualBoarder);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//vLines[c].setFillColor(gridColor);
			}

			scenes[csi].hLines.reserve((H + 2 * virtualBoarder) / gap + 2);
			tempVect.x = W + 2 * virtualBoarder; tempVect.y = 1;
			for (int c = 0; c <= (H + 2 * virtualBoarder) / gap; c++) {
				scenes[csi].hLines.emplace_back(tempVect);
			}
			for (int c = 0; c < scenes[csi].hLines.size(); c++) {
				scenes[csi].hLines[c].setPosition(-virtualBoarder, -virtualBoarder - (int)(-virtualBoarder) % gap + c * gap);
				//gridColor.a = 20 + (c % 5 == 0) * 15;
				//hLines[c].setFillColor(gridColor);
			}

			colorBrightLineGrid(index);
		}

		allSqrDesign(index);

		/*Vectors*/
		scenes[index].comp.reserve(3);
		scenes[index].allEnds.reserve(3);
		scenes[index].virSerial.reserve(3);
		scenes[index].virSprite.reserve(3);
		scenes[index].virSerialShift.reserve(3);
		scenes[index].allEndCircles.reserve(17);
	}
	void initializeGUI()
	{
		Entity::setFont("assets/Fonts/CalibriL_1.ttf");

		//renderWinInit();
		{
			W = sf::VideoMode::getDesktopMode().width * 0.85;
			H = W * 9 / 16;
			if (H < c_toolColWidth * 7) H = c_toolColWidth * 7;

			//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
			/*sf::RenderWindow app*/// *(;
			app.create(sf::VideoMode((unsigned int)W, (unsigned int)H), "CircuitSim", sf::Style::Default, sf::ContextSettings(0, 0, 8));

			W = app.getSize().x; H = app.getSize().y;
			app.setVerticalSyncEnabled(1);
			app.setFramerateLimit(60);
			srand(time(NULL));

			//std::cout << "my lcd: " << sf::VideoMode::getDesktopMode().width << ", " << sf::VideoMode::getDesktopMode().height;
			app.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - W / 2, sf::VideoMode::getDesktopMode().height / 2 - H / 2 - 50));
		}

		scenes.emplace_back();
		csi = scenes.size() - 1 * (!!scenes.size());
		initializeScene(csi);

		//loadTextures();
		{
			compTex[Entity::Cap].loadFromFile("assets/Images/Cap.png");
			compTex[Entity::Cur].loadFromFile("assets/Images/Cur.png");
			compTex[Entity::Dod].loadFromFile("assets/Images/Dod.png");
			compTex[Entity::Ind].loadFromFile("assets/Images/Ind.png");
			compTex[Entity::Res].loadFromFile("assets/Images/Res.png");
			compTex[Entity::SwO].loadFromFile("assets/Images/SwO.png");
			compTex[Entity::Vol].loadFromFile("assets/Images/Vol.png");
			compTex[Entity::SwC].loadFromFile("assets/Images/SwC.png");
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
			toolCol.setSize(sf::Vector2f(c_toolColWidth, scenes[csi].view.getSize().y));

			/*Color*/
			toolCol.setFillColor(sf::Color(0, 0, 0, 120));
			ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));

			/*Sprites*/
			for (int c = 0; c < (Entity::noOfComps - 1); c++) {
				ToolSpr[c].setTexture(compTex[c]);
				ToolSpr[c].setOrigin(compTex[c].getSize().x / 2, 0);
				//compSpr[c].setRotation(45);
				ToolSprPOS[c].x = c_toolColWidth / 2;
				ToolSprPOS[c].y = c * c_toolColWidth + (int)(c_toolColWidth - compTex[c].getSize().y) / 2;
				ToolSpr[c].setPosition(scenes[csi].view.getCenter().x - scenes[csi].view.getSize().x / 2 + ToolSprPOS[c].x, scenes[csi].view.getCenter().y - scenes[csi].view.getSize().y / 2 + ToolSprPOS[c].y);
			}
		}

		//void nodePicDesign();
		{
			nodePic.setOrigin(4, 4);
			nodePic.setFillColor(CircuitGUI::normalCompColor);
		}

		selSqrDesign();

		/*Vectors*/
		//scenes.reserve(2);
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

			if (scenes[csi].comp.size() != 0)
			{
				ABCD = areaofCollection(true);
				OutputImage.create(abs(ABCD.width) + 30, abs(ABCD.height) + 30);
			}
			else { OutputImage.create(10, 10); }

			for (int c = 0; c < scenes[csi].comp.size(); c++) {
				sf::Vector2f tempEndNode = scenes[csi].comp[c].getEndPos();
				compBound = scenes[csi].comp[c].bounds;

				sf::Image tempCompImg;
				//tempCompImg.createMaskFromColor(sf::Color(23, 24, 25));

				if (scenes[csi].comp[c].angle == 0) {
					tempCompImg = compTex[scenes[csi].comp[c].serial].copyToImage();
				}
				else if (scenes[csi].comp[c].angle == 180) {
					tempCompImg = compTex[scenes[csi].comp[c].serial].copyToImage();
					tempCompImg.flipVertically();
					tempCompImg.flipHorizontally();
				}
				else {
					sf::Image tempTempCompImg(compTex[scenes[csi].comp[c].serial].copyToImage());
					tempCompImg.create(tempTempCompImg.getSize().y, tempTempCompImg.getSize().x);

					for (int j = 0; j < tempTempCompImg.getSize().y; j++) {
						for (int i = 0; i < tempTempCompImg.getSize().x; i++) {
							tempCompImg.setPixel(j, i, tempTempCompImg.getPixel(i, j));
						}
					}
					tempCompImg.flipHorizontally();


					if (scenes[csi].comp[c].angle == 90) {
						;
					}
					else if (scenes[csi].comp[c].angle == 270) {
						tempCompImg.flipVertically();
						tempCompImg.flipHorizontally();

					}
				}

				bool rotBool = ((((int)scenes[csi].comp[c].angle) / 90) % 2 == 1);

				int fakeGap = 15 - (scenes[csi].comp[c].serial == 5) * 5;

				float OffX = compBound.left - ABCD.left - !rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;
				float OffY = compBound.top - ABCD.top - rotBool * (fakeGap + (75 - 2 * fakeGap - 30) / 2) + 15;

				for (int j = rotBool * fakeGap; j < tempCompImg.getSize().y - rotBool * fakeGap; j++) {
					for (int i = !rotBool * fakeGap; i < tempCompImg.getSize().x - !rotBool * fakeGap; i++) {
						if (tempCompImg.getPixel(i, j).a < 10) continue;
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

			scenes.emplace_back(filepath);
			csi = scenes.size() - 1 * (!!scenes.size());
			initializeScene();

			std::ifstream input(filepath);
			/*int fileNo = 0;
			std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
			std::ifstream input(fileDir + std::to_string(fileNo) + fileType);
			while (input.good()) {
				input.close();
				input.open(fileDir + std::to_string(++fileNo) + fileType);
			}
			input.open(fileDir + std::to_string(fileNo - 1) + fileType);*/

			int no = 0;
			input >> no;
			scenes[csi].comp.reserve(no + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
				input >> S >> X >> Y >> A;
				scenes[csi].comp.emplace_back(S % (Entity::noOfComps + 1), trim(X), trim(Y), ((A % 360) / 90) * 90);
			}
			input.close();


			// Selecting
			scenes[csi].virSerial.reserve(scenes[csi].comp.size());
			scenes[csi].virSprite.reserve(scenes[csi].comp.size());
			for (int vv = 0; vv < scenes[csi].comp.size(); vv++) {
				scenes[csi].virSerial.emplace_back(vv);
				//virSprite.emplace_back(comp[virSerial.back()].sprite);
				scenes[csi].virSprite.emplace_back();	// We wouldn't be able to see them anyways

				//virSprite.back().setColor(tempDimColor);	// We wouldn't be able to see them anyways
			}

			// Centering
			sf::FloatRect virArea = areaofCollection(true);
			sf::Vector2f offSet(scenes[csi].view.getCenter().x - (virArea.left + (int)(virArea.width / 2)), scenes[csi].view.getCenter().y - (virArea.top + (int)(virArea.height / 2)));
			offSet = trim(offSet);
			for (int c = 0; c < scenes[csi].comp.size(); c++)
			{
				scenes[csi].comp[c].x = (int)trim(scenes[csi].comp[c].x + offSet.x);
				scenes[csi].comp[c].y = (int)trim(scenes[csi].comp[c].y + offSet.y);
				scenes[csi].comp[c].stimuli();
			}

			// EndNodes
			updateAllEnds();
			updateEndCircles();
			updateAllSqr();
		}

		void savef(const std::string& filepath) {

			std::ofstream output;

			//int fileNo = 0;
			//std::string fileDir = "Saved-Projects/Project-", fileType = ".txt";
			//std::ifstream test(fileDir + std::to_string(fileNo) + fileType);
			//while (test.good()) {
			//	test.close();
			//	test.open(fileDir + std::to_string(++fileNo) + fileType);
			//}
			//output.open(fileDir + std::to_string(fileNo) + fileType);


			output.open(filepath);

			std::string tempStr;
			tempStr = std::to_string((int)scenes[csi].comp.size()) + "\n";
			int size = (int)scenes[csi].comp.size();
			for (int c = 0; c < size; c++) {
				tempStr += std::to_string(scenes[csi].comp[c].serial) + "\t" + std::to_string((int)scenes[csi].comp[c].x) + "\t" + std::to_string((int)scenes[csi].comp[c].y) + "\t" + std::to_string((int)scenes[csi].comp[c].angle) + "\n";
			}
			output << tempStr;
			output.close();


			WCHAR hello[260] = { 0 };
			for (int i = 0; i < 260 && i < filepath.length(); i++) hello[i] = filepath[i];
			ShellExecute(NULL, NULL, hello, NULL, NULL, SW_SHOWNORMAL);
		}

		void copyf() {

			std::ofstream output;
			std::string tempStr(std::to_string((int)scenes[csi].virSerial.size()) + "\n");

			int size = (int)scenes[csi].virSerial.size();
			for (int c = 0; c < size; c++) {
				tempStr += std::to_string(scenes[csi].comp[scenes[csi].virSerial[c]].serial) + "\t" + std::to_string((int)scenes[csi].comp[scenes[csi].virSerial[c]].x) + "\t" + std::to_string((int)scenes[csi].comp[scenes[csi].virSerial[c]].y) + "\t" + std::to_string((int)scenes[csi].comp[scenes[csi].virSerial[c]].angle) + "\n";
			}

			//clipboard << tempStr;
			sf::Clipboard::setString(tempStr);

		}

		void pastef() {

			std::string inString;
			//clipboard >> inString;
			inString = sf::Clipboard::getString();
			std::vector<int> integers; integers.reserve(21);


			bool negative = 0, numStarted = 0;
			for (int c = 0, x = 0, currentCharacter = 0; c < inString.size(); c++) {
				currentCharacter = (int)inString[c];

				if ('0' <= currentCharacter && currentCharacter <= '9') {
					numStarted = 1;
					x *= 10;
					x += currentCharacter - (int)'0';
				}
				else {
					if (currentCharacter == '-') negative = 1;
					else if (numStarted) {
						if (negative) x *= -1;
						integers.emplace_back(x);
						negative = 0;
						x = 0;
						numStarted = 0;
					}
				}
			}
			//std::cout << "\n"; for (int c = 0; c < integers.size(); c++) std::cout << integers[c] << " ";


			int OffsetX = 0, OffsetY = 0, count = 0;
			for (int i = 1; i + 2 < integers.size(); i += 4, count++)
			{
				OffsetX += integers[i + 1];
				OffsetY += integers[i + 2];
			}
			if (count) {
				OffsetX /= count;
				OffsetY /= count;

				OffsetX = scenes[csi].view.getCenter().x - OffsetX;
				OffsetY = scenes[csi].view.getCenter().y - OffsetY;
			}

			int compSizeBefore = scenes[csi].comp.size();
			scenes[csi].comp.reserve(abs(integers[0]) + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; 1 + c + 4 <= integers.size();) {
				S = abs(integers[++c]);
				X = integers[++c] + OffsetX;
				Y = integers[++c] + OffsetY;
				A = abs(integers[++c]);
				scenes[csi].comp.emplace_back(S % (Entity::noOfComps/* + 1*/), trim(X), trim(Y), ((A % 360) / 90) * 90);
			}


			scenes[csi].virSerial.clear();
			scenes[csi].virSprite.clear();
			scenes[csi].virSerialShift.clear();

			scenes[csi].virSerial.reserve(scenes[csi].comp.size() - compSizeBefore);
			scenes[csi].virSprite.reserve(scenes[csi].comp.size() - compSizeBefore);

			for (int vv = 0; vv < (scenes[csi].comp.size() - compSizeBefore); vv++) {
				scenes[csi].virSerial.emplace_back(vv + compSizeBefore);
				scenes[csi].virSprite.emplace_back(scenes[csi].comp[scenes[csi].virSerial.back()].sprite);
				scenes[csi].virSprite.back().setColor(tempDimColor);
			}

			/*
			sf::FloatRect virArea = areaofCollection(false);
			sf::Vector2f offSet(view.getCenter().x - (virArea.left + (int)(virArea.width / 2)), view.getCenter().y - (virArea.top + (int)(virArea.height / 2)));
			std::cout << "\nBad :" << (virArea.left + (int)(virArea.width / 2)) << ", " << (virArea.top + (int)(virArea.height / 2));
			for (int v = 0; v < virSerial.size(); v++)
			{
				comp[virSerial[v]].x = trim(comp[virSerial[v]].x + offSet.x);
				comp[virSerial[v]].y = trim(comp[virSerial[v]].y + offSet.y);
				comp[virSerial[v]].stimuli();
			}*/

		}

		void rotatef() {

			for (int v = 0; v < scenes[csi].virSerial.size(); v++) {
				scenes[csi].comp[scenes[csi].virSerial[v]].angle += 90;
				scenes[csi].comp[scenes[csi].virSerial[v]].angle -= (int)(scenes[csi].comp[scenes[csi].virSerial[v]].angle / 360) * 360;
				scenes[csi].comp[scenes[csi].virSerial[v]].stimuli();
			}

		}

		void deletef() {

			//5th Implementation	| O(nlogn) Currently Fastest
			//(erase - remove idiom) approach
			auto iter = std::remove_if(scenes[csi].comp.begin(), scenes[csi].comp.end(), [&](const auto& elem) {
				return std::binary_search(scenes[csi].virSerial.begin(), scenes[csi].virSerial.end(), &elem - &scenes[csi].comp[0]);
				});
			scenes[csi].comp.erase(iter, scenes[csi].comp.end());

			scenes[csi].virSerial.clear();
			scenes[csi].virSprite.clear();
			scenes[csi].virSerialShift.clear();


			// 4th Implementation	| O(n^2) but mostly really fast
			/*// As complexity of eraseing one element is same as eraseing 100's(more than one) of "CONSECUTIVE" elements
			//   | e.g.
			//   |--> complexity of erase(v.begin() + i) IS EQUAL TO complexity of erase(v.begin() + i, v.begin() + i + 1000)
			//
			// So we will erase all the elements in a sorted set together
			//   | e.g.
			//   |--> in [1,2,3,4, 7,8,9,10,11, 51], we will delete them in three separate chunks (1,2,3,4), (7,8,9,10,11) and (51). And each chunk will be erased at a time.
			//   |--> so instead of erasing 10 elements sepreatly we erase three whole chunks (saving us time of erasing 7(10-3) elements).

			int totalKills = 0;
			while (!!scenes[csi].virSerial.size()) {
				int v = 1;
				for (; v < scenes[csi].virSerial.size(); v++)
					if (scenes[csi].virSerial[v - 1] + 1 < scenes[csi].virSerial[v]) break; // Chhecking for consecutive numbers

				scenes[csi].comp.erase(scenes[csi].comp.begin() + (scenes[csi].virSerial[0] - totalKills), scenes[csi].comp.begin() + (scenes[csi].virSerial[0] - totalKills + v));
				scenes[csi].virSerial.erase(scenes[csi].virSerial.begin(), scenes[csi].virSerial.begin() + v);

				totalKills += v;
			}

			scenes[csi].virSerial.clear();
			scenes[csi].virSprite.clear();
			scenes[csi].virSerialShift.clear();*/

			// 3rd Implementation	| Works but Slow
			/*while (0 < virSerial.size()) {
				if (comp.size() > 0) comp.erase(comp.begin() + virSerial[0]);
				if (virSprite.size() > 0) virSprite.erase(virSprite.begin());

				for (int c = 1; c < virSerial.size(); c++) {
					if (virSerial[c] > virSerial[0]) virSerial[c]--;
				}

				virSerial.erase(virSerial.begin());
			}*/

			// 2nd Implementation	| Works but Slower
			/* {
				for (int v = 0; v < virSerial.size(); v++)
					comp.erase(comp.begin() + (virSerial[v] - v)); //assuming vir or virSerial is sorted(ascendingly)
			}*/

			// 1st Implementation	| Works but Slooooooooooower
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

	namespace Responses
	{
		void followCoursor() {
			/*static const sf::Vector2f offsetHold[4] = {
				{0, -2},
				{2, 0},
				{0, 2},
				{-2, 0}
			};*/
			sf::FloatRect virArea = scenes[csi].allSqr.getGlobalBounds();
			sf::Vector2f offsetPos((int)cursorInSim().x - (virArea.left + (int)(virArea.width / 2)), (int)cursorInSim().y - (virArea.top + (int)(virArea.height / 2)));

			for (int v = 0; v < scenes[csi].virSprite.size(); v++)
				scenes[csi].virSprite[v].setPosition(offsetPos.x + scenes[csi].comp[scenes[csi].virSerial[v]].x, offsetPos.y + scenes[csi].comp[scenes[csi].virSerial[v]].y);

			offsetPos = trim(offsetPos);

			bool moveAll = 1;
			sf::Vector2f temp;
			for (int v = 0; v < scenes[csi].virSerial.size(); v++) {
				temp.x = offsetPos.x + scenes[csi].comp[scenes[csi].virSerial[v]].x;
				temp.y = offsetPos.y + scenes[csi].comp[scenes[csi].virSerial[v]].y;

				if (occupiedAt(scenes[csi].virSerial[v], temp, true)) { moveAll = 0; break; }
			}
			if (moveAll) {
				for (int v = 0; v < scenes[csi].virSerial.size(); v++) {
					scenes[csi].comp[scenes[csi].virSerial[v]].x += (int)offsetPos.x; // += (were "=" before)
					scenes[csi].comp[scenes[csi].virSerial[v]].y += (int)offsetPos.y; // += (were "=" before)
					scenes[csi].comp[scenes[csi].virSerial[v]].stimuli();
				}
			}

		}

		void selection() {

			/*Sel Sqr*/
			selSqr.setSize(cursorInSim() - selSqr.getPosition());


			/*Selection*/
			for (int c = 0; c < scenes[csi].comp.size(); c++) {
				//cout << std::count(virSerial.begin(), virSerial.end(), c);
				//bool bi = std::binary_search(virSerial.begin(), virSerial.end(), c);
				//auto mk = std::find(virSerial.begin(), virSerial.end(), c);
				int v = std::lower_bound(scenes[csi].virSerial.begin(), scenes[csi].virSerial.end(), c) - scenes[csi].virSerial.begin();
				bool compFound = 0;
				if (v < scenes[csi].virSerial.size()) compFound = (scenes[csi].virSerial[v] == c);
				//bool compFound = 0;
				//int v = 0;
				//for (; v < CircuitGUI::virSerial.size() && c >= CircuitGUI::virSerial[v]; v++) { // if(CircuitGUI::virSerial[v]) break; // Ensures the virSerial is Sorted by breaking at the right time
				//	if (c == CircuitGUI::virSerial[v]) { compFound = 1; break; }
				//}
				//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));


				if (scenes[csi].comp[c].bounds.intersects(selSqr.getGlobalBounds())) {
					if (!compFound)
					{
						scenes[csi].virSerial.insert(scenes[csi].virSerial.begin() + v, c);
						//CircuitGUI::virSprite.insert(CircuitGUI::virSprite.begin() + v, CircuitGUI::comp[c].sprite);
						//CircuitGUI::virSprite[v].setColor(tempDimColor);

						/*virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
						virSprite.back().setColor(tempDimColor);*/
					}
				}
				else if (compFound)
				{
					scenes[csi].virSerial.erase(scenes[csi].virSerial.begin() + v);
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

			updateAllSqr();
		}
	}

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