// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
#pragma once

//#include <iostream>
//#include <windows.h> //To Open file(txt/PNG) just after creating(Save as...) it
#include <fstream>

#include "LOG.hpp"
#include "SFML/Graphics.hpp"

#include "Circuit_Global.hpp"
#include "Circuit_Entity.hpp"
#include "Circuit_Wire.hpp"
#include "Resource_Manager.hpp"

//extern sf::Texture compTex[8];
extern std::vector<sf::Texture> compTex;


namespace CircuitGUI {

	/*Constants*/
	inline const int gap = 15;
	inline const sf::Vector2f zero(0, 0);
	inline const sf::Color normalCompColor(230, 230, 230);

	inline bool Occupied = false;
	inline bool darkLightMode = false;
	inline sf::Color tempDimColor(60, 60, 60/*150, 150, 150*/);

	/*Textures*/
	//void loadTextures()
	inline Resource Resource_Logo;
	inline Resource Resource_Font;
	inline std::vector<Resource> Resource_Images;

	inline sf::Event evnt;
	inline sf::RenderWindow app;
	//renderWinInit();
	inline sf::View view;
	//viewInit();


	/*Cursor*/
	inline float mouseHoldX, mouseHoldY;
	inline time_t click = clock(); // Time passed since Click
	inline bool Click(int Sensitivity = 3) {
		return (((float)clock() - (float)click) < 100) && !!(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && abs(mouseHoldX - (float)sf::Mouse::getPosition(app).x) <= Sensitivity && abs(mouseHoldY - (float)sf::Mouse::getPosition(app).y) <= Sensitivity);
	}
	inline sf::Vector2f cursorInSim() {
		return app.mapPixelToCoords(sf::Mouse::getPosition(app));
	}
	inline sf::Vector2f onScreen(float X, float Y) {
		return sf::Vector2f(view.getCenter().x - view.getSize().x / 2 + X, view.getCenter().y - view.getSize().y / 2 + Y);
		//return app.mapPixelToCoords(sf::Vector2i((int)X, (int)Y));
	}

	
	// Warning: Clears the vec
	inline void str_to_vecInt(const std::string& str, std::vector<int>& vec)
	{
		bool negative = 0, numStarted = 0;
		vec.clear();
		vec.reserve(23); // HardCode

		for (int c = 0, x = 0, temp = 0; c < str.size(); c++) {
			temp = (int)str[c];
			char chrecter = temp;

			if ('0' <= temp && temp <= '9') {
				numStarted = 1;
				x *= 10;
				x += temp - (int)'0';
			}
			else {
				if (temp == '-') negative = 1;
				else if (numStarted) {
					if (negative) x *= -1;
					vec.emplace_back(x);
					negative = 0;
					x = 0;
					numStarted = 0;
				}
			}

			if (numStarted == true && c == str.size() - 1)
			{
				if (negative) x *= -1;
				vec.emplace_back(x);
			}
		}
	}
	inline float trim(float num, int wrt = gap) {
		return num - (int)num % wrt;
	}
	inline sf::Vector2f trim(sf::Vector2f vec, int wrt = gap) {
		vec.y = trim(vec.y, wrt);
		vec.x = trim(vec.x, wrt);

		return vec;
	}


	/*Grid*/
	inline int virtualBoarder = 80;
	inline std::vector<sf::RectangleShape> vLines;
	inline std::vector<sf::RectangleShape> hLines;
	inline sf::Color gridColor(100, 105, 110, 20);
	inline sf::Color backColor(23, 24, 25);
	inline void colorGrid() {
		if (darkLightMode)CircuitGUI::gridColor = { 212, 232, 247, 50 };
		else CircuitGUI::gridColor = { 100, 105, 110, 20 }; //R,G,B,a
	}
	inline void colorBackground() {
		if (darkLightMode) CircuitGUI::backColor = { 36, 133, 202 };
		else CircuitGUI::backColor = { 23, 24, 25 }; //R,G,B
	}
	inline void drawGrid() {
		for (int c = 0; c < vLines.size(); c++) { app.draw(vLines[c]); }
		for (int c = 0; c < hLines.size(); c++) { app.draw(hLines[c]); }
	}
	//void initializeGrid()


	/*Drag*/
	inline bool dragBool = 0;
	inline float viewX = 0, viewY = 0;
	inline float verX = 0, verY = 0;
	inline float horX = 0, horY = 0;
	inline int verBrightCount = 5, horBrightCount = 5;
	inline void iniDrag() {
		mouseHoldX = (float)sf::Mouse::getPosition(app).x; mouseHoldY = (float)sf::Mouse::getPosition(app).y;
		viewX = view.getCenter().x, viewY = view.getCenter().y;
		verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
		horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
	}
	inline void Drag() {
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
	inline void colorBrightLineGrid() {
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
	inline std::vector<Entity> comp;
	inline std::vector<int> virSerial;
	inline std::vector<sf::Sprite> virSprite;
	inline std::vector<int> virSerialShift;
	inline std::vector<Wire> wires;
	inline std::vector<sf::Vector2f> allEnds;
	inline std::vector<sf::CircleShape> allEndCircles;
	inline std::vector<int> visibleComps;
	inline void drawComp() {
		//for (int c = 0; c < CircuitGUI::comp.size(); c++) { CircuitGUI::comp[c].draw(CircuitGUI::app); }
		for (int i = 0; i < visibleComps.size(); i++) {
			comp[visibleComps[i]].draw(app);
		}
	}
	inline void drawVirSprites() {
		if (Occupied)
			for (int v = 0; v < CircuitGUI::virSprite.size(); v++)
				CircuitGUI::app.draw(CircuitGUI::virSprite[v]);
	}
	inline void drawNodes() {
		for (int e = 0; e < CircuitGUI::allEndCircles.size(); e++) { CircuitGUI::app.draw(CircuitGUI::allEndCircles[e]); }
	}
	inline void drawBoarders() {
		for (int v = 0; v < CircuitGUI::virSerial.size(); v++)
			CircuitGUI::app.draw(CircuitGUI::comp[CircuitGUI::virSerial[v]].boarder);
	}
	inline void drawWires() {
		for (int i = 0; i < wires.size(); i++)
			wires[i].draw(app);
	}
	inline void updateAllEnds() {

		int nodeCount = 0;
		for (int c = 0; c < comp.size(); c++) {
			static sf::Vector2f tempEnd;

			for (int cc = 0; cc < 2; cc++) {

				//tempEnd
				if (cc == 0) { /*Front*/ tempEnd.x = comp[c].x; tempEnd.y = comp[c].y; }
				if (cc == 1) { /*Rear*/  tempEnd = comp[c].getEndPos(); }

				bool found = 0;
				for (int e = 0; e < nodeCount/*allEnds.size()*/; e++) {
					if (tempEnd == allEnds[e]) {
						found = 1;
						break;
					}
				}

				if (!found) {

					if (allEnds.empty())
					{
						allEnds.emplace_back(tempEnd);
						nodeCount++;
					}
					else if (nodeCount < allEnds.size())
						allEnds[nodeCount++] = tempEnd;
					else
					{
						allEnds.emplace_back(tempEnd);
						nodeCount++;
					}
				}

			}
		}

		allEnds.erase(allEnds.begin() + nodeCount, allEnds.end());
	}
	inline bool makingWire()
	{
		if (wires.empty())
			return false;
		else if (wires.back().isStopped())
			return false;
		else
			return true;
	}
	

	struct quadTree {
		static const int limit = 5;
		bool isSubDivided = false;
		unsigned int size = 0;
		std::vector<int> arr;
		sf::FloatRect area;
		sf::RectangleShape rectDraw;								// [0 1]
		quadTree* sub[4] = { nullptr, nullptr ,nullptr ,nullptr };	// [2 3]

		quadTree() {
			arr.reserve(quadTree::limit);
		}
	};
	inline quadTree qt;
	inline void qtDelete(quadTree& box) {
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

		}

		box.size = 0;
		box.arr.clear();
		box.isSubDivided = false;
		box.area = { 0,0,0,0 };
		box.rectDraw.setSize(zero);
	}
	inline void qtAdd(int c, quadTree& box) {
		if (box.area.intersects(comp[c].bounds)) {
			if ((box.arr.size() < quadTree::limit) || (std::min(box.area.width, box.area.height) < gap * 5)) {
				box.arr.emplace_back(c);
				box.size++;// = box.arr.size();
			}
			else {
				if (box.isSubDivided == false) { // HardCode (5)
					// Sub-divide
					box.isSubDivided = true;
					int qtHalfWidth = (int)(box.area.width / 2), qtHalfHeight = (int)(box.area.height / 2);
					box.sub[0] = new quadTree; box.sub[0]->area.width = qtHalfWidth; box.sub[0]->area.height = qtHalfHeight; box.sub[0]->area.left = box.area.left;					box.sub[0]->area.top = box.area.top;
					box.sub[1] = new quadTree; box.sub[1]->area.width = qtHalfWidth; box.sub[1]->area.height = qtHalfHeight; box.sub[1]->area.left = box.area.left + qtHalfWidth;	box.sub[1]->area.top = box.area.top;
					box.sub[2] = new quadTree; box.sub[2]->area.width = qtHalfWidth; box.sub[2]->area.height = qtHalfHeight; box.sub[2]->area.left = box.area.left;					box.sub[2]->area.top = box.area.top + qtHalfHeight;
					box.sub[3] = new quadTree; box.sub[3]->area.width = qtHalfWidth; box.sub[3]->area.height = qtHalfHeight; box.sub[3]->area.left = box.area.left + qtHalfWidth;	box.sub[3]->area.top = box.area.top + qtHalfHeight;

					for (int i = 0; i < 4; i++) {
						box.sub[i]->rectDraw.setSize(sf::Vector2f(box.sub[i]->area.width, box.sub[i]->area.height));
						box.sub[i]->rectDraw.setPosition(sf::Vector2f(box.sub[i]->area.left, box.sub[i]->area.top));
						box.sub[i]->rectDraw.setFillColor(sf::Color(box.sub[i]->rectDraw.getPosition().y - box.sub[i]->rectDraw.getPosition().x,
																	box.sub[i]->rectDraw.getPosition().x + box.sub[i]->rectDraw.getLocalBounds().height,
																	box.sub[i]->rectDraw.getLocalBounds().width - box.sub[i]->rectDraw.getPosition().x, 100));
					}


					// Add qtArr in subdivided areas
					for (int i = 0; i < quadTree::limit; i++)
					{
						qtAdd(box.arr[i], *box.sub[0]);
						qtAdd(box.arr[i], *box.sub[1]);
						qtAdd(box.arr[i], *box.sub[2]);
						qtAdd(box.arr[i], *box.sub[3]);
					}
				}

				qtAdd(c, *box.sub[0]);
				qtAdd(c, *box.sub[1]);
				qtAdd(c, *box.sub[2]);
				qtAdd(c, *box.sub[3]);
				box.size++;
			}

			//return 1;
		}
		//else return 0;
	}
	inline sf::FloatRect areaofCollection(int collection = false, bool* sucess_ptr = nullptr); // Forward Declerarion
	inline void qtUpdate() {

		qtDelete(qt);
		qt.area = areaofCollection(true);
		qt.rectDraw.setSize(sf::Vector2f(qt.area.width, qt.area.height));
		qt.rectDraw.setPosition(sf::Vector2f(qt.area.left, qt.area.top));
		qt.rectDraw.setFillColor(sf::Color(qt.rectDraw.getPosition().y - qt.rectDraw.getPosition().x,
											qt.rectDraw.getPosition().x + qt.rectDraw.getLocalBounds().height,
											qt.rectDraw.getLocalBounds().width - qt.rectDraw.getPosition().x, 100));

		int compSize = comp.size();
		if (compSize <= quadTree::limit) {
			qt.size = compSize;
			for (int c = 0; c < compSize; c++)
				qt.arr.emplace_back(c);
		}
		else {
			for (int c = 0; c < compSize; c++) //HardCode
				qtAdd(c, qt);
		}
	}
	inline void qtWrite(const quadTree& box = qt, int taaabbss = 0) {

		for (int t = 0; t < taaabbss; t++) std::cout << "\t";
		if (box.isSubDivided) std::cout << "`";
		std::cout << "(" << box.size << "): ";
		for (int i = 0; i < box.arr.size(); i++)
			std::cout << box.arr[i] << ", ";
		std::cout << "\b\b \n";

		if (box.isSubDivided) {
			qtWrite(*box.sub[0], taaabbss + 1);
			qtWrite(*box.sub[1], taaabbss + 1);
			qtWrite(*box.sub[2], taaabbss + 1);
			qtWrite(*box.sub[3], taaabbss + 1);
		}

	}
	inline void qtDraw(quadTree& box) {

		app.draw(box.rectDraw);

		if (box.isSubDivided) {
			qtDraw(*box.sub[0]);
			qtDraw(*box.sub[1]);
			qtDraw(*box.sub[2]);
			qtDraw(*box.sub[3]);
		}

	}
	inline void qtExtract(const sf::FloatRect& searchArea, std::vector<int>& output, const quadTree& box = qt) {

		if (&box == &qt)
			output.clear();

		if (searchArea.intersects(box.area)) {

			if (box.isSubDivided == false)
			{
				// Check if box resides entirly in searchArea
				// If Yes, then add all the elements of box.arr in the box
				if (searchArea.contains(box.area.left, box.area.top) &&
					searchArea.contains(box.area.left + box.area.width, box.area.top + box.area.height))
				{
					for (int i = 0; i < box.arr.size(); i++)
						if (std::binary_search(output.begin(), output.end(), box.arr[i]) == false)
						{
							auto insert_pos = std::lower_bound(output.begin(), output.end(), box.arr[i]);
							output.emplace(insert_pos, box.arr[i]);
						}
				}
				else
				{
					for (int i = 0; i < box.arr.size(); i++)
						if (searchArea.intersects(comp[box.arr[i]].bounds))
							if (std::binary_search(output.begin(), output.end(), box.arr[i]) == false)
							{
								auto insert_pos = std::lower_bound(output.begin(), output.end(), box.arr[i]);
								output.emplace(insert_pos, box.arr[i]);
							}
				}
				
				
				//{
				//	int insert_pos = std::lower_bound(output.begin(), output.end(), box.arr[i]) - output.begin();
				//	if (insert_pos < output.size() && output[insert_pos] != box.arr[i])
				//		output.insert (output.begin() + insert_pos, box.arr[i]);
				//}
				
			}
			else {
				qtExtract(searchArea, output, *box.sub[0]);
				qtExtract(searchArea, output, *box.sub[1]);
				qtExtract(searchArea, output, *box.sub[2]);
				qtExtract(searchArea, output, *box.sub[3]);
			}
		}

	}


	inline bool occupiedAt(Entity e, const sf::Vector2f& At, bool ignoreAllVir = false) {
		e.x = At.x; e.y = At.y;
		e.stimuli();

		std::vector<int> vec;
		qtExtract(e.bounds, vec);

		if (ignoreAllVir)
			for (int v = virSerial.size() - 1; v >= 0; v--) {
				int index = lower_bound(vec.begin(), vec.end(), virSerial[v]) - vec.begin();
				if (index < vec.size() && (vec[index] == virSerial[v]))
					vec.erase(vec.begin() + index);
			}

		int count = 0;
		for (int i = 0; i < vec.size(); i++) {
			count = 0;

			if (e.x == comp[vec[i]].x &&
				e.y == comp[vec[i]].y)
				count++;

			if (e.x == comp[vec[i]].getEndPos().x &&
				e.y == comp[vec[i]].getEndPos().y)
				count++;

			if (e.getEndPos().x == comp[vec[i]].x &&
				e.getEndPos().y == comp[vec[i]].y)
				count++;

			if (e.getEndPos().x == comp[vec[i]].getEndPos().x &&
				e.getEndPos().y == comp[vec[i]].getEndPos().y)
				count++;

			if (count > 1) { Occupied = true; return 1; }
		}

		if (count == 1) { Occupied = false; return 0; }
		
		Occupied = (!vec.empty());
		return (!vec.empty());

	}
	inline sf::FloatRect areaofCollection(int collection, bool* sucess_ptr) {
		sf::FloatRect compBound;
		sf::FloatRect ABCD;
		
		if (collection == 0) {
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
		else if (collection == 1) {
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
		else if (collection == 2) {

			if (wires.empty() == false) {
				bool sucess = false;

				for (auto& w : wires) {
					w.clean();
					compBound = w.bounds(&sucess);

					if (sucess == true)
						break;
				}

				if (sucess == false) {
					if (sucess_ptr != nullptr)
						*sucess_ptr = false;

					return ABCD;
				}
				else {
					if (sucess_ptr != nullptr)
						*sucess_ptr = true;
				}

				ABCD = compBound;
			}
			else {
				if (sucess_ptr != nullptr)
					*sucess_ptr = false;

				return ABCD;
			}

			for (int v = 0; v < wires.size(); v++) {
				wires[v].clean();

				bool sucess = false;
				compBound = wires[v].bounds(&sucess);
				if (sucess == false)
					continue;

				if (compBound.left < ABCD.left) { ABCD.width += ABCD.left - compBound.left; ABCD.left = compBound.left; }
				if (compBound.top < ABCD.top) { ABCD.height += ABCD.top - compBound.top; ABCD.top = compBound.top; }
				if (compBound.left + compBound.width > ABCD.left + ABCD.width)	ABCD.width = compBound.left - ABCD.left + compBound.width;
				if (compBound.top + compBound.height > ABCD.top + ABCD.height)	ABCD.height = compBound.top - ABCD.top + compBound.height;
			}
		}

		if (sucess_ptr != nullptr)
			*sucess_ptr = true;
		return ABCD;
	}
	inline void updateVisibleVector()
	{
		sf::FloatRect searcharea(sf::Vector2f(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2), view.getSize());

		qtExtract(searcharea, visibleComps);
	}


	inline void colorEntityboarder() {

		sf::Color temp;

		if (darkLightMode)
			temp = { 0, 255, 85 };
		else
			temp = { 0/*200*/, 204, 102 };

		Entity::setboarderDesgin(temp);

		for (int c = 0; c < comp.size(); c++)
			comp[c].boarder.setOutlineColor(temp);

	}
	inline sf::CircleShape nodePic(4, 15);
	//void nodePicDesign()
	inline sf::RectangleShape selSqr;
	inline void selSqrDesign() {
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
	inline void drawSelSqr() {
		app.draw(selSqr);
	}
	inline sf::RectangleShape allSqr;
	inline void allSqrDesign() {
		allSqr.setFillColor(sf::Color::Transparent);
		allSqr.setOutlineThickness(1.0f);

		if (darkLightMode) allSqr.setOutlineColor(sf::Color(0, 0, 0/*0, 51, 102*/));//0, 255, 85
		else allSqr.setOutlineColor(sf::Color(249, 140, 31));
	}
	inline void updateAllSqr() {

		sf::FloatRect virArea(areaofCollection(false));

		allSqr.setSize(sf::Vector2f(virArea.width, virArea.height));
		allSqr.setPosition(sf::Vector2f(virArea.left, virArea.top));

	}
	inline void drawAllSqr() {
		if (virSerial.size() > 1)
			app.draw(allSqr);
	}


	/*ToolBox*/
	inline const float c_toolColWidth = 100;
	inline sf::Vector2f toolWinRestPos(0, 0);
	inline sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, CircuitGUI::view.getSize().y));
	inline sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(c_toolColWidth, c_toolColWidth));
	//void toolBoxInit()
	inline sf::Sprite ToolSpr[Entity::no_of_Comp - 1];
	inline sf::Vector2f ToolSprPOS[Entity::no_of_Comp - 1];
	inline void updatePosToolBox()
	{
		toolWinRestPos = onScreen(0, 0);
		for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
			ToolSpr[c].setPosition(onScreen(ToolSprPOS[c].x, ToolSprPOS[c].y));
	}
	inline void updateEndCircles()
	{
		while (allEnds.size() < allEndCircles.size())
			allEndCircles.erase(allEndCircles.begin() + allEnds.size(), allEndCircles.end());


		while (allEndCircles.size() < allEnds.size())
			allEndCircles.emplace_back(nodePic);


		for (int e = 0; e < allEndCircles.size(); e++)
			allEndCircles[e].setPosition(allEnds[e]);

		//cout << "\n" << allEnds.size();
	}
	inline void drawToolColumn(bool MInTool, bool MIntool) {
		if (MInTool) {
			app.draw(toolCol);

			for (int c = 0; c < (Entity::no_of_Comp - 1); c++) { ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15)); }
			for (int c = 0; c < (Entity::no_of_Comp - 1); c++) { app.draw(ToolSpr[c]); }
		}
		else {
			for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
				ToolSpr[c].setColor(sf::Color::Transparent);
		}

		if (MIntool) app.draw(ToolBoxLittleBox);
	}


	inline void updateThemeColors()
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
	inline void initializeGUI()
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


#ifdef _DEBUG
			std::ifstream window_size("temp_files/win_size.txt");
			if (window_size.good())
			{
				int arr[4] = { W, H, app.getPosition().x, app.getPosition().y };
				window_size >> arr[0] >> arr[1] >> arr[2] >> arr[3];
				window_size.close();

				app.setSize(sf::Vector2u(arr[0], arr[1]));
				app.setPosition(sf::Vector2i(arr[2], arr[3]));
			}
#endif
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
			Resource_Images.resize(Entity::no_of_Comp + 1);

			Resource_Images[Entity::Cap].SetAll(IDR_CAP, "BMP");
			Resource_Images[Entity::Cur].SetAll(IDR_CUR, "BMP");
			Resource_Images[Entity::Dod].SetAll(IDR_DOD, "BMP");
			Resource_Images[Entity::Ind].SetAll(IDR_IND, "BMP");
			Resource_Images[Entity::Res].SetAll(IDR_RES, "BMP");
			Resource_Images[Entity::SwO].SetAll(IDR_SWO, "BMP");
			Resource_Images[Entity::Vol].SetAll(IDR_VOL, "BMP");
			Resource_Images[Entity::SwC].SetAll(IDR_SWC, "BMP");
			Resource_Images.back().SetAll(IDR_DEF, "BMP");


			compTex.resize(Entity::no_of_Comp + 1);
			for (int i = 0; i < compTex.size(); i++)
				compTex[i].loadFromMemory(Resource_Images[i].GetResource().ptr, Resource_Images[i].GetResource().size_bytes);

			for (auto& tex : compTex)
				tex.setSmooth(true);
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

			/*{
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
		}

		//toolBoxInit();
		{
			/*Color*/
			CircuitGUI::toolCol.setFillColor(sf::Color(0, 0, 0, 120));
			CircuitGUI::ToolBoxLittleBox.setFillColor(sf::Color(160, 160, 160, 120));

			/*Sprites*/
			for (int c = 0; c < (Entity::no_of_Comp - 1); c++) {
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
			nodePic.setOrigin(nodePic.getRadius(), nodePic.getRadius());
			nodePic.setFillColor(normalCompColor);
		}
		
		/*Vectors*/ {
			comp.reserve(3);
			allEnds.reserve(3);
			virSerial.reserve(3);
			virSprite.reserve(3);
			virSerialShift.reserve(3);
			wires.reserve(8);
			allEndCircles.reserve(17);
			visibleComps.reserve(70);
		}

		selSqrDesign();

		allSqrDesign();
	}


	namespace Options
	{
		inline void openf(const std::string& filepath) {

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
			wires.clear();

			// Input from file
			int no = 0;
			input >> no;
			comp.reserve(no + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
				input >> S >> X >> Y >> A;
				comp.emplace_back(S, trim(X), trim(Y), ((A % 360) / 90) * 90);
			}

			// Wires
			wires.reserve(3);
			std::string line;
			bool started_reading_wires = false;
			while (std::getline(input, line)) {
				//LOG("\n\ngetline(): " << line);

				if (started_reading_wires)
					wires.emplace_back(line);

				if (line == "#Wires:")
					started_reading_wires = true;
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

			for (auto& w : wires)
				w.move(w.initial() + offSet);


			qtUpdate();
			updateVisibleVector();
		}

		inline void savef(const std::string& file) {

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
				tempStr += std::to_string(comp[c].getSerial()) + "\t" + std::to_string((int)comp[c].x) + "\t" + std::to_string((int)comp[c].y) + "\t" + std::to_string((int)comp[c].angle) + "\n";
			}
			output << tempStr;

			// Wires
			output << "\n#Wires:\n";
			for (auto& w : wires)
				output << w.serialize();


			output.close();

			WCHAR hello[260] = { 0 };
			for (int i = 0; i < 260 && i < file.length(); i++) hello[i] = file[i];
			
			ShellExecute(NULL, NULL, hello, NULL, NULL, SW_SHOWNORMAL);
		}

		inline void saveAsImage(const std::string& filepath) {

			/*try
			{
				LOG(filepath);
				int len;
				int slength = (int)filepath.length() + 1;
				len = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), slength, 0, 0);
				wchar_t* buf = new wchar_t[len];
				MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), slength, buf, len);
				std::wstring r(buf);
				delete[] buf;
				const wchar_t* widecstr = r.c_str();
			
				//ShellExecute(NULL, NULL, L"Saved-Images", NULL, NULL, SW_SHOWNORMAL);
				ShellExecute(NULL, NULL, widecstr, NULL, NULL, SW_SHOWNORMAL);
			}
			catch (const std::exception&)
			{
				LOG("Shell connot open floder  \"Saved - Images\"");
			}*/

			sf::Image image;
			sf::FloatRect Bounds_of_all_Entities = areaofCollection(true);
			{
				bool sucess = false;
				sf::FloatRect boundWire = areaofCollection(2, &sucess); // bound_of_all_Wires
				if (sucess == true) {
					if (boundWire.left < Bounds_of_all_Entities.left)
						{ Bounds_of_all_Entities.width += Bounds_of_all_Entities.left - boundWire.left; Bounds_of_all_Entities.left = boundWire.left; }
					if (boundWire.top < Bounds_of_all_Entities.top)
						{ Bounds_of_all_Entities.height += Bounds_of_all_Entities.top - boundWire.top; Bounds_of_all_Entities.top = boundWire.top; }
					if (boundWire.left + boundWire.width > Bounds_of_all_Entities.left + Bounds_of_all_Entities.width)
						Bounds_of_all_Entities.width = boundWire.left - Bounds_of_all_Entities.left + boundWire.width;
					if (boundWire.top + boundWire.height > Bounds_of_all_Entities.top + Bounds_of_all_Entities.height)
						Bounds_of_all_Entities.height = boundWire.top - Bounds_of_all_Entities.top + boundWire.height;
				}
			}


			if (comp.empty())
				image.create(10, 10, backColor);
			else
				image.create(Bounds_of_all_Entities.width + 2 * gap, Bounds_of_all_Entities.height + 2 * gap, backColor);

			// Comps
			for (int c = 0; c < comp.size(); c++) {

				int x_offset = gap + comp[c].bounds.left - Bounds_of_all_Entities.left;
				int y_offset = gap + comp[c].bounds.top - Bounds_of_all_Entities.top;

				static sf::Image img;

				switch ((int)comp[c].angle) {
				case 0:
				{
					//std::cout << "\n"<<(int)(comp[c].sprite.getTexture()->getSize().x/2.0f - gap);
					x_offset -= (int)(comp[c].sprite.getTexture()->getSize().x / 2.0f - gap);
					img = comp[c].sprite.getTexture()->copyToImage();
					break;
				}
				case 180:
				{
					x_offset -= (int)(comp[c].sprite.getTexture()->getSize().x / 2.0f - gap);
					y_offset -= (int)(comp[c].sprite.getTexture()->getSize().y - gap * 5);

					img = (comp[c].sprite.getTexture()->copyToImage());
					img.flipVertically();
					img.flipHorizontally();
					break;
				}
				case 90:
				case 270:
				{
					static sf::Image rot_img;
					rot_img = comp[c].sprite.getTexture()->copyToImage();
					img.create(rot_img.getSize().y, rot_img.getSize().x);

					for (int j = 0; j < rot_img.getSize().y; j++)
						for (int i = 0; i < rot_img.getSize().x; i++)
							img.setPixel(j, i, rot_img.getPixel(i, j));
					img.flipHorizontally();

					if (comp[c].angle == 90) {
						x_offset -= (int)(comp[c].sprite.getTexture()->getSize().y - gap * 5);
						y_offset -= (int)(comp[c].sprite.getTexture()->getSize().x / 2.0f - gap);
					}
					else if (comp[c].angle == 270) {
						y_offset -= (int)(comp[c].sprite.getTexture()->getSize().x / 2.0f - gap);

						img.flipVertically();
						img.flipHorizontally();
					}

					break;
				}
				default: break;
				}


				for (int y = 0; y < img.getSize().y; y++)
					for (int x = 0; x < img.getSize().x; x++)
					{
						static sf::Color col;
						col = img.getPixel(x, y);
						if (col != sf::Color::Transparent)
							image.setPixel(x + x_offset, y + y_offset, col);
					}
			}

			// Wires
			for (auto& w : wires) {

				bool sucess = false;
				sf::FloatRect bound(w.bounds(&sucess));

				if (sucess == false || w.noOfDrawRects() < 1)
					continue;

				auto& vec = *w.getRectVector();


				for (auto& rec : vec) {

					int x_offset = /*gap +*/ rec.getGlobalBounds().left - Bounds_of_all_Entities.left;
					int y_offset = /*gap +*/ rec.getGlobalBounds().top  - Bounds_of_all_Entities.top;

					int w = std::abs(rec.getSize().x);
					int h = std::abs(rec.getSize().y);

					if (abs(w) > 1 && abs(h) > 1) // HardCode
					{
						for (int y = 0; y < h; y++)
							for (int x = 0; x < w; x++)
								image.setPixel(x + x_offset, y + y_offset, normalCompColor);
					}
				}

			}


			image.saveToFile(filepath);

			//int picNo = 0;
			//std::string picDir = "Saved-Images/Untitled-", picType = ".png";
			//sf::Image test;
			//while (test.loadFromFile(picDir + std::to_string(picNo++) + picType)) {
			//	;
			//	LOG( "\n" << picNo);
			//}
			//image.saveToFile(picDir + std::to_string(picNo - 1) + picType);


			WCHAR Filepath_w[260] = { 0 }; // HardCode (size of arr)
			for (int i = 0; i < 260 && i < filepath.length(); i++) Filepath_w[i] = filepath[i];
			ShellExecute(NULL, NULL, Filepath_w, NULL, NULL, SW_SHOWNORMAL);
		}

		inline void copyf() {

			std::ofstream output;
			std::string tempStr(std::to_string((int)virSerial.size()) + "\n");

			// Components
			{
				int size = (int)virSerial.size();
				for (int c = 0; c < size; c++) {
					tempStr += std::to_string(comp[virSerial[c]].getSerial()) + "\t" + std::to_string((int)comp[virSerial[c]].x) + "\t" + std::to_string((int)comp[virSerial[c]].y) + "\t" + std::to_string((int)comp[virSerial[c]].angle) + "\n";
				}
			}

			// Wires
			{
				sf::FloatRect area = selSqr.getGlobalBounds();
				tempStr += "\n#Wires:\n"; // HardCode
				for (auto& w : wires)
					if (w.intersectes(area))
						tempStr += w.serialize();
			}

			sf::Clipboard::setString(tempStr);
		}

		inline void pastef() {

			std::vector<int> integers; integers.reserve(21);
			std::string inString(sf::Clipboard::getString());

			size_t wire_start = inString.find_first_of('#');
			
			std::string comp_str(inString.substr(0, wire_start));
			std::string wire_str(inString.substr(wire_start + 8, inString.size() - wire_start)); // HardCode (7) length("#Wires:\n")
			//LOG("\nPasting:\ncomp: " << comp_str << "hello");
			//LOG("\nPasting:\nwire: " << wire_str << "mello");
			
			str_to_vecInt(comp_str, integers);

			// Offset
			int OffsetX = 0, OffsetY = 0;
			{
				int count = 0;
				for (int i = 1; i + 2 < integers.size(); i += 4, count++)
				{
					OffsetX += integers[i + 1];
					OffsetY += integers[i + 2];
				}
				if (count) {
					OffsetX /= count;
					OffsetY /= count;

					OffsetX = (int)trim((int)view.getCenter().x - OffsetX);
					OffsetY = (int)trim((int)view.getCenter().y - OffsetY);
				}
			}


			// Comps
			{
				int noOfComponentsBefore = comp.size();
				comp.reserve(abs(integers[0]) + 10); // 10 extra
				for (int c = 0, S = 0, X = 0, Y = 0, A = 0; 1 + c + 4 <= integers.size();) {
					S = abs(integers[++c]);
					X = integers[++c] + OffsetX;
					Y = integers[++c] + OffsetY;
					A = abs(integers[++c]);
					comp.emplace_back(S /*% (Entity::no_of_Comp)*/, trim(X), trim(Y), ((A % 360) / 90) * 90);
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

			qtUpdate();
			updateVisibleVector();

			// Wires
			{
				LOG("\nwire paste:\n");
				sf::Vector2f offSet(OffsetX, OffsetY);
				size_t wire_stop;
				std::string sub;
				for (int count = 0; count < 10000 && wire_str.size() > 1; count++) // HardCode limit(10000)
				{
					wire_stop = wire_str.find_first_of('\n');
					//LOG("\nstop : " << wire_stop << "\n");
					sub = wire_str.substr(0, wire_stop + 1);
					//LOG(sub);
					if (sub.size() > 3) // HardCode (3) --To avoid making wires for string with only \n character
					{
						wires.emplace_back(sub);
						wires.back().move(wires.back().initial() + offSet);
					}
					else LOG("\n[Warrning] Wire sub.size() > 3 | sub = \""<< sub <<"\" | Options::Paste()");
					wire_str.erase(wire_str.begin(), wire_str.begin() + wire_stop + 1);
				}

			}

		}

		inline void rotatef() {

			for (int v = 0; v < virSerial.size(); v++) {
				comp[virSerial[v]].angle += 90;
				comp[virSerial[v]].angle -= (int)(comp[virSerial[v]].angle / 360) * 360;
				comp[virSerial[v]].stimuli();
			}

			qtUpdate();
			updateVisibleVector();
		}

		inline void deletef() {

			//erase - remove idiom
			auto iter = std::remove_if(comp.begin(), comp.end(), [&](const auto& elem) {
				return std::binary_search(virSerial.begin(), virSerial.end(), &elem - &comp[0]);
				});
			comp.erase(iter, comp.end());

			// Wires
			sf::FloatRect area = selSqr.getGlobalBounds();
			for (int i = 0; i < wires.size(); i++) {
				if (wires[i].intersectes(area)) {
					wires.erase(wires.begin() + i);
					i--;
				}
			}

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

			qtUpdate();
			updateVisibleVector();
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
