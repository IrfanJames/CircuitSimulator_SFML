#pragma once
//#include "Circuit_Global.h"
#include "SFML/Graphics.hpp"

extern sf::Texture compTex[8];


namespace CircuitGUI {

	/*Constants*/
	const sf::Color normalCompColor(230, 230, 230), tempDimColor(150, 150, 150);

	/*Textures*/
	void loadTextures() {
		Entity::Cap;
		compTex[Entity::Cap].loadFromFile("assets/Images/Cap.png");
		compTex[Entity::Cur].loadFromFile("assets/Images/Cur.png");
		compTex[Entity::Dod].loadFromFile("assets/Images/Dod.png");
		compTex[Entity::Ind].loadFromFile("assets/Images/Ind.png");
		compTex[Entity::Res].loadFromFile("assets/Images/Res.png");
		compTex[Entity::SwO].loadFromFile("assets/Images/SwO.png");
		compTex[Entity::Vol].loadFromFile("assets/Images/Vol.png");
		compTex[Entity::SwC].loadFromFile("assets/Images/SwC.png");
	}


	sf::RenderWindow app;
	void renderWinInit() {
		W = sf::VideoMode::getDesktopMode().width * 0.74;
		H = sf::VideoMode::getDesktopMode().height * 0.7;

		//RenderWindow app(VideoMode(W, H), "CircuitSim", Style::Fullscreen, ContextSettings(0));
		/*sf::RenderWindow app*/// *(;
		app.create(sf::VideoMode((unsigned int)W, (unsigned int)H), "CircuitSim", sf::Style::Default, sf::ContextSettings(0, 0, 8));

		W = app.getSize().x; H = app.getSize().y;
		app.setVerticalSyncEnabled(1);
		app.setFramerateLimit(60);
		srand(time(NULL));

		//std::cout << "my lcd: " << sf::VideoMode::getDesktopMode().width << ", " << sf::VideoMode::getDesktopMode().height;
		app.setPosition(sf::Vector2i(250, 100));
	}
	sf::View view;
	void viewInit() {
		view.setCenter(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2));
		view.setSize((sf::Vector2f)app.getSize());
		//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), sf::Vector2f(app.getSize().x * 2, app.getSize().y * 2));
		//sf::View view(sf::Vector2f(app.getSize().x / 2, app.getSize().y / 2), (sf::Vector2f)app.getSize());
	}


	/*Cursor*/
	float mouseHoldX, mouseHoldY;
	time_t click = clock(); // Time passed since Click
	bool Click(int Sensitivity) {
		return (((float)clock() - (float)click) < 100) && !!(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && abs(mouseHoldX - (float)sf::Mouse::getPosition(app).x) <= Sensitivity && abs(mouseHoldY - (float)sf::Mouse::getPosition(app).y) <= Sensitivity);
	}
	/*Extra*/
	sf::Vector2f cursorInSim() {
		return app.mapPixelToCoords(sf::Mouse::getPosition(app));
	}
	sf::Vector2f onScreen(float X, float Y) {
		return sf::Vector2f(view.getCenter().x - view.getSize().x / 2 + X, view.getCenter().y - view.getSize().y / 2 + Y);
		//return app.mapPixelToCoords(sf::Vector2i((int)X, (int)Y));
	}
	float trim(float num, int wrt) {
		return num - (int)num % wrt;
	}


	std::vector<Entity> comp;
	void drawComp() {
		for (int c = 0; c < CircuitGUI::comp.size(); c++) { CircuitGUI::comp[c].draw(CircuitGUI::app); }
	}

	std::vector<int> virSerial;
	std::vector<sf::Sprite> virSprite;
	void drawVirSprites() {
		for (int v = 0; v < CircuitGUI::virSprite.size(); v++)
			CircuitGUI::app.draw(CircuitGUI::virSprite[v]);
	}

	std::vector<int> virSerialShift;

	std::vector<sf::Vector2f> allEnds;
	std::vector<sf::CircleShape> allEndCircles;
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
	bool occupiedAt(int Index, const sf::Vector2f& At) {

		int noCount = 0;
		for (int c = 0; c < CircuitGUI::comp.size(); c++) {
			if (c == Index || abs(CircuitGUI::comp[c].x - At.x) >= 200 || abs(CircuitGUI::comp[c].y - At.y) >= 200) continue; // gap-hardcode

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
			if (indexEntity.bounds.intersects(CircuitGUI::comp[c].bounds)) { Occupied = 1; return 1; }

		}

		Occupied = 0; return 0;
	}


	/*Grid*/
	const int gap = 15, virtualBoarder = 80;
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;
	sf::Color gridColor(100, 105, 110, 20);
	sf::Color backColor(23, 24, 25);
	void initializeGrid() {
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


	sf::CircleShape nodePic(4, 15);
	void nodePicDesign() {
		nodePic.setOrigin(4, 4);
		nodePic.setFillColor(CircuitGUI::normalCompColor);
	}

	sf::RectangleShape selSqr;
	void selSqrDesign() {
		selSqr.setFillColor(sf::Color(66, 135, 245, 60));
		selSqr.setOutlineThickness(1.0f);
		selSqr.setOutlineColor(sf::Color(66, 135, 245));
	}
	void drawSelSqr() {
		CircuitGUI::app.draw(CircuitGUI::selSqr);
	}

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


		horBrightY -= hLines[horBrightCount].getPosition().y;
		if (horBrightY > gap * 0.9) horBrightCount--; else if (horBrightY < -gap * 0.9) horBrightCount++;
		horBrightCount = abs((horBrightCount < 1) * (5 + horBrightCount % 5) + (1 <= horBrightCount) * ((horBrightCount - 1) % 5 + 1)) % 6;
		//cout << "\n" << horBrightCount << "\n";
	}
	void colorGrid() {
		for (int c = 0; c < vLines.size(); c++) {
			gridColor.a = 20 + ((c + verBrightCount) % 5 == 0) * 15;
			vLines[c].setFillColor(gridColor);
		}
		for (int c = 0; c < hLines.size(); c++) {
			gridColor.a = 20 + ((c + horBrightCount) % 5 == 0) * 15;
			hLines[c].setFillColor(gridColor);
		}
	}
	void drawGrid() {
		for (int c = 0; c < CircuitGUI::vLines.size(); c++) { CircuitGUI::app.draw(CircuitGUI::vLines[c]); }
		for (int c = 0; c < CircuitGUI::hLines.size(); c++) { CircuitGUI::app.draw(CircuitGUI::hLines[c]); }
	}


	/*ToolBox*/
	const float c_toolColWidth = 100;
	sf::Vector2f toolWinRestPos(0, 0);
	sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, CircuitGUI::view.getSize().y));
	sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(c_toolColWidth, c_toolColWidth));
	sf::Sprite ToolSpr[7]; //noOfComps-1
	sf::Vector2f ToolSprPOS[7]; //noOfComps-1
	void toolBoxInit()
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
	void updatePosToolBox()
	{
		toolWinRestPos = onScreen(0, 0);
		for (int c = 0; c < (Entity::noOfComps - 1); c++) ToolSpr[c].setPosition(onScreen(ToolSprPOS[c].x, ToolSprPOS[c].y));
	}
	void updateEndCircles()
	{
		while (allEnds.size() < allEndCircles.size())
			allEndCircles.pop_back();


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


	void init()
	{
		Entity::setFont("assets/Fonts/CalibriL_1.ttf");

		renderWinInit();
		viewInit();
		loadTextures();

		initializeGrid();
		toolBoxInit();
		nodePicDesign();
		selSqrDesign();

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

		void openf(const std::string& filepath) {

			virSerial.clear();
			virSprite.clear();
			virSerialShift.clear();

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
			comp.reserve(no + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; c < no; c++) {
				input >> S >> X >> Y >> A;
				comp.emplace_back(S % (Entity::noOfComps + 1), trim(X, gap), trim(Y, gap), ((A % 360) / 90) * 90);
			}
			input.close();

		}

		void savef(const std::string& file) {

			std::cout << "\nCtrl + S\n";

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

			std::string inString;
			//clipboard >> inString;
			inString = sf::Clipboard::getString();
			//cout << inString;

			std::vector<int> integers; integers.reserve(9);


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

			comp.reserve(abs(integers[0]) + 10); // 10 extra
			for (int c = 0, S = 0, X = 0, Y = 0, A = 0; 1 + c + 4 <= integers.size();) {
				S = integers[++c];
				X = integers[++c];
				Y = integers[++c];
				A = integers[++c];
				comp.emplace_back(S % (Entity::noOfComps + 1), trim(X, gap), trim(Y, gap), ((A % 360) / 90) * 90);
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
		}
	}

}

/*
int total = 0;
			while (!!virSerial.size())
			{
				int v = 1;
				for (; v < virSerial.size(); v++)
					if (virSerial[v - 1] + 1 < virSerial[v]) break;


				comp.erase(comp.begin() + virSerial[0] - total, comp.begin() + virSerial[0] - total + v);
				virSerial.erase(virSerial.begin(), virSerial.begin() + v);

				total += v;
			}
*/