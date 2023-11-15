
/*
	* Colapse all Scopes - it'll be easier
	* Ctrl + M + A in Visual Studio
*/

#include "Circuit_GUI.hpp"
#include "Circuit_Global.hpp"
#include "Resource_Manager.hpp"
#include "LOG.hpp"

//#include <windows.h>
//#include <objbase.h>


namespace CircuitGUI {

	// Constants
	const int gap = 15;
	const sf::Vector2f zero(0, 0);
	const sf::Color normalCompColor(230, 230, 230);

	bool Occupied = false;
	bool darkLightMode = false;
	sf::Color tempDimColor(60, 60, 60/*150, 150, 150*/);

	// Textures
	std::vector<Resource> Resource_Images;
	Resource Resource_Logo;
	Resource Resource_Font;
	//void loadTextures()

	sf::Event evnt;
	sf::RenderWindow app;
	//renderWinInit();
	sf::View view;
	//viewInit();


	/*Cursor*/
	sf::Vector2f mouseHold;
	sf::Vector2f mouseOffSet;
	time_t click = clock(); // Time passed since Click
	bool Click(int Sensitivity = 3) {
		return (
			((float)clock() - (float)click) < 100) &&
			!!(!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			abs(mouseHold.x - (float)sf::Mouse::getPosition(app).x) <= Sensitivity &&
			abs(mouseHold.y - (float)sf::Mouse::getPosition(app).y) <= Sensitivity );
	}
	sf::Vector2f cursorInSim() {
		return app.mapPixelToCoords(sf::Mouse::getPosition(app));
	}
	sf::Vector2f onScreen(float X, float Y) {
		return sf::Vector2f(view.getCenter().x - view.getSize().x / 2 + X, view.getCenter().y - view.getSize().y / 2 + Y);
		//return app.mapPixelToCoords(sf::Vector2i((int)X, (int)Y));
	}


	// Warning: Clears the vec
	void str_to_vecInt(const std::string& str, std::vector<int>& vec)
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
	float trim(float num, int wrt) {
		return num - (int)num % wrt;
	}
	sf::Vector2f trim(sf::Vector2f vec, int wrt) {
		vec.y = trim(vec.y, wrt);
		vec.x = trim(vec.x, wrt);

		return vec;
	}


	// Grid
	int virtualBoarder = 80;
	std::vector<sf::RectangleShape> vLines;
	std::vector<sf::RectangleShape> hLines;
	sf::Color gridColor(100, 105, 110, 20);
	sf::Color backColor(23, 24, 25);
	void colorGrid()
	{
		if (darkLightMode)
			gridColor = { 212, 232, 247, 50 };
		else
			gridColor = { 100, 105, 110, 20 }; // R,G,B,a
	}
	void colorBackground()
	{
		if (darkLightMode)
			backColor = { 36, 133, 202 };
		else
			backColor = { 23, 24, 25 }; // R,G,B
	}
	void drawGrid()
	{
		for (auto& rectangle : vLines)
			app.draw(rectangle);

		for (auto& rectangle : hLines)
			app.draw(rectangle);
	}
	//void initializeGrid()


	// Drag
	bool dragBool = 0;
	sf::Vector2f viewPos(0,0);
	sf::Vector2f ver(0,0);
	sf::Vector2f hor(0,0);
	int verBrightCount = 5, horBrightCount = 5;
	void iniDrag() {
		mouseHold = (sf::Vector2f)sf::Mouse::getPosition(app);
		viewPos = view.getCenter();
		ver = vLines[0].getPosition();
		hor = hLines[0].getPosition();
	}
	void Drag() {
		
		view.setCenter( viewPos + mouseHold - (sf::Vector2f)sf::Mouse::getPosition(app) );

		float newVerY = ver.y + mouseHold.y - (float)sf::Mouse::getPosition(app).y;
		float newHorX = hor.x + mouseHold.x - (float)sf::Mouse::getPosition(app).x;


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


	// Vectors
	std::vector<Entity> comp;
	std::vector<int> virSerial;
	std::vector<sf::Sprite> virSprite;
	std::vector<int> virSerialShift;
	std::vector<Wire> wires;
	std::vector<sf::Vector2f> allEnds;
	std::vector<int> visibleComps;
	std::vector<int> visibleEndNodes;
	std::vector<int> visibleBoarders;

	void drawComp() {

		for (int i : visibleComps)
			comp[i].draw(app);

		//for (auto& c : comp)
		//	c.draw(app);
	}
	void drawVirSprites() {
		if (Occupied)
			for (auto& sprite : virSprite)
				app.draw(sprite);
	}
	void drawNodes() {

		;

		//for (int i : visibleEndNodes) {
		//	nodePic.setPosition(allEnds[i]);
		//	app.draw(nodePic);
		//}
	}
	void drawBoarders() {

		//for (int v : virSerial)
		//	app.draw(comp[v].boarder);

		for (int v : visibleBoarders)
			app.draw(comp[v].boarder);
	}
	void drawWires() {

		//for (int i = 0; i < wires.size(); i++)
		//	wires[i].draw(app);

		for (auto& wire : wires)
			wire.draw(app);
	}
	void updateAllEnds_old() {

		//	allEnds.clear();

		//	int nodeCount = 0;
		//	for (int c = 0; c < comp.size(); c++) {
		//		static sf::Vector2f tempEnd;

		//		for (int cc = 0; cc < 2; cc++) {

		//			//tempEnd
		//			if (cc == 0) { /*Front*/ tempEnd.x = comp[c].x; tempEnd.y = comp[c].y; }
		//			if (cc == 1) { /*Rear*/  tempEnd = comp[c].getEndPos(); }

		//			bool found = 0;
		//			for (int e = 0; e < nodeCount/*allEnds.size()*/; e++) {
		//				if (tempEnd == allEnds[e]) {
		//					found = 1;
		//					break;
		//				}
		//			}

		//			if (!found) {

		//				if (allEnds.empty())
		//				{
		//					allEnds.emplace_back(tempEnd);
		//					nodeCount++;
		//				}
		//				else if (nodeCount < allEnds.size())
		//					allEnds[nodeCount++] = tempEnd;
		//				else
		//				{
		//					allEnds.emplace_back(tempEnd);
		//					nodeCount++;
		//				}
		//			}

		//		}
		//	}

		//	allEnds.erase(allEnds.begin() + nodeCount, allEnds.end());

	}
	void updateAllEnds_a() {
		bool Print = false;

		int ttttttttttt = allEnds.size();
		if (Print) LOG("Before: "); if (Print) for (auto& vec : allEnds) LOG_VEC2(vec);
		allEnds.clear();

		static std::vector<int> end1;
		static std::vector<int> end2;

		end1.reserve(comp.size());
		end2.reserve(comp.size());

		for (int c = 0; c < comp.size(); c++) {
			end1.emplace_back(c);
			end2.emplace_back(c);
		}

		// Nodes
		static const int rectSize = 3;
		static sf::Vector2f tempVec2;
		static sf::FloatRect searchArea;
		static std::vector<int> intersects;

		// Front End
		if (Print) LOG("\nupdateAllEnds2 for\n");


		for (int v = 0; v < 2; v++) {
			auto vec = &end1;

			if (v == 0) { vec = &end1; if (Print)LOG("\n11111111111111 end1"); }
			if (v == 1) { vec = &end2; if (Print)LOG("\n22222222222222 end2"); }

			for (int i = 0; i < (*vec).size(); i++)
				//for (int i = (*vec).size() - !((*vec).empty()); i >= 0; i--) // Just Iterating in Reverse // For Optimization sake // Optimization related to deleting in std::vector
			{
				int Index = (*vec)[i];
				int x = 0, y = 0;

				// x, y
				if (v == 0) {
					x = comp[Index].x;
					y = comp[Index].y;
				}
				else {
					x = comp[Index].getEndPos().x;
					y = comp[Index].getEndPos().y;
				}
				allEnds.emplace_back(x, y);
				int Node = allEnds.size() - !allEnds.empty();

				//
				if (v == 0)
					comp[Index].node1 = Node;
				else
					comp[Index].node2 = Node;

				// finding intersecting comp. by QuadTree
				searchArea = { x - rectSize / 2.0f, y - rectSize / 2.0f, rectSize, rectSize };
				qtExtract(searchArea, intersects);


				if (Print) LOG("--------------\ni = " << i << ", allEnds.back(): ");
				if (Print) LOG_VEC2(allEnds.back());
				if (Print) LOG_VEC(intersects);

				for (int j = 0; j < intersects.size(); j++)
				{
					if (intersects[j] == Index) continue;

					tempVec2.x = comp[intersects[j]].x;
					tempVec2.y = comp[intersects[j]].y;

					if (allEnds.back() == tempVec2) // Try Front End
					{
						comp[intersects[j]].node1 = Node;

						auto it = std::find(end1.begin(), end1.end(), intersects[j]);
						if (it != end1.end())
							end1.erase(it);
					}
					else if (allEnds.back() == comp[intersects[j]].getEndPos()) // Try Back End
					{
						comp[intersects[j]].node2 = Node;

						auto it = std::find(end2.begin(), end2.end(), intersects[j]);
						if (it != end2.end())
							end2.erase(it);
					}

				}


				// current's Front End
				{
					auto it = std::find((*vec).begin(), (*vec).end(), Index);
					if (it != (*vec).end()) {
						(*vec).erase(it);
						i--;
					}
				}

				if (Print) {
					LOG("\nallEnds.size() : " << allEnds.size() << "\n");
					LOG("end1:");
					LOG_VEC(end1);
					LOG("\nend2:");
					LOG_VEC(end2);
				}
			}
		}

		if (Print) {

			LOG("\n\n-------------");
			for (int c = 0; c < comp.size(); c++)
				LOG("\n" << c << ": " << comp[c].node1 << ", " << comp[c].node2);

			LOG("\nBefore: " << ttttttttttt);
			LOG("\nAfter:  " << allEnds.size());
		}


		if (Print) LOG("\n\nAfter:  "); if (Print) for (auto& vec : allEnds) LOG_VEC2(vec);
	}
	void updateAllEnds_print() {
		bool Print = true;

		int ttttttttttt = allEnds.size();
		if (Print) LOG("Before: "); if (Print) for (auto& vec : allEnds) LOG_VEC2(vec);
		allEnds.clear();

		static std::vector<bool> end1;
		static std::vector<bool> end2;

		end1.resize(comp.size()/*, false*/);
		end2.resize(comp.size()/*, false*/);

		for (int i = 0; i < comp.size(); i++) {
			end1[i] = false;
			end2[i] = false;
		}

		// Nodes
		static const int rectSize = 6;
		static sf::Vector2f tempVec2;
		static sf::FloatRect searchArea;
		static std::vector<int> intersects;

		// Front End
		if (Print) LOG("\nupdateAllEnds2 for\n");


		for (int v = 0; v < 2; v++) {
			auto vec = &end1;

			if (v == 0) { vec = &end1; if (Print) LOG("\n11111111111111 end1"); }
			if (v == 1) { vec = &end2; if (Print) LOG("\n22222222222222 end2"); }

			for (int i = 0; i < (*vec).size(); i++)
				//for (int i = (*vec).size() - !((*vec).empty()); i >= 0; i--) // Just Iterating in Reverse // For Optimization sake // Optimization related to deleting in std::vector
			{
				//int Index = (*vec)[i];
				if ((*vec)[i] == true) continue;
				int x = 0, y = 0;

				// x, y
				if (v == 0) {
					x = comp[i].x;
					y = comp[i].y;
				}
				else {
					x = comp[i].getEndPos().x;
					y = comp[i].getEndPos().y;
				}
				allEnds.emplace_back(x, y);
				int Node = allEnds.size() - !allEnds.empty();

				//
				if (v == 0)
					comp[i].node1 = Node;
				else
					comp[i].node2 = Node;

				// finding intersecting comp. by QuadTree
				searchArea = { x - rectSize / 2.0f, y - rectSize / 2.0f, rectSize, rectSize };
				if (Print) {
					LOG("--------------\n");
					LOG("\nleft:\t" << searchArea.left);
					LOG("\ntop:\t" << searchArea.top);
					LOG("\nwidth:\t" << searchArea.width);
					LOG("\nheight:\t" << searchArea.height << "\n");
				}
				qtExtract(searchArea, intersects);


				if (Print) {
					LOG("--------------\ni = " << i << ", allEnds.back(): ");
					LOG_VEC2(allEnds.back());
					LOG_VEC(intersects);
				}

				for (int j = 0; j < intersects.size(); j++)
				{
					if (intersects[j] == i) continue;

					tempVec2.x = comp[intersects[j]].x;
					tempVec2.y = comp[intersects[j]].y;

					if (allEnds.back() == tempVec2) // Try Front End
					{
						comp[intersects[j]].node1 = Node;

						//auto it = std::find(end1.begin(), end1.end(), intersects[j]);
						//if (it != end1.end())
						//	end1.erase(it);
						end1[intersects[j]] = true;
					}
					else if (allEnds.back() == comp[intersects[j]].getEndPos()) // Try Back End
					{
						comp[intersects[j]].node2 = Node;

						//auto it = std::find(end2.begin(), end2.end(), intersects[j]);
						//if (it != end2.end())
						//	end2.erase(it);
						end2[intersects[j]] = true;
					}

				}


				// current's Front End
				{
					//auto it = std::find((*vec).begin(), (*vec).end(), Index);
					//if (it != (*vec).end()) {
					//	(*vec).erase(it);
					//	i--;
					//}
					(*vec)[i] = true;
				}

				if (Print) {
					LOG("\nallEnds.size() : " << allEnds.size() << "\n");
					LOG("end1:");
					LOG_VEC(end1);
					LOG("\nend2:");
					LOG_VEC(end2);
				}
			}
		}


		if (Print) {

			LOG("\n\n-------------");
			for (int c = 0; c < comp.size(); c++)
				LOG("\n" << c << ": " << comp[c].node1 << ", " << comp[c].node2);

			LOG("\nBefore: " << ttttttttttt);
			LOG("\nAfter:  " << allEnds.size());
		}

		if (Print) 
		{
			LOG("\n\nAfter:  ");
			for (int i = 0; i < allEnds.size(); i++)
			{
				LOG("\n" << i << ": ");
				LOG_VEC2(allEnds[i]);
			}
		}
	}
	void updateAllEnds() {

		allEnds.clear();

		static std::vector<bool> end1;
		static std::vector<bool> end2;

		end1.assign(comp.size(), false); // assign() changes the size (_Count) of the vector
		end2.assign(comp.size(), false); // and assigns the value (_Val) in one operation

		// Nodes
		/*static*/ const int rectSize = 10;
		/*static*/ sf::Vector2f tempVec2;
		/*static*/ sf::FloatRect searchArea;
		static std::vector<int> intersects;

		// Front End

		for (int v = 0; v < 2; v++) {
			auto vec = &end1;
			
			if (v == 0) vec = &end1;
			if (v == 1) vec = &end2;

			for (int i = 0; i < (*vec).size(); i++)
				//for (int i = (*vec).size() - !((*vec).empty()); i >= 0; i--) // Just Iterating in Reverse // For Optimization sake // Optimization related to deleting in std::vector
			{
				//int Index = (*vec)[i];
				if ((*vec)[i] == true) continue;

				// TODO: this
				//if (comp[i].getSerial() == Entity::SwO) continue;

				int x = 0, y = 0;

				// x, y
				if (v == 0) {
					x = comp[i].x;
					y = comp[i].y;
				}
				else {
					x = comp[i].getEndPos().x;
					y = comp[i].getEndPos().y;
				}
				allEnds.emplace_back(x, y);
				int Node = allEnds.size() - !allEnds.empty();

				//
				if (v == 0)
					comp[i].node1 = Node;
				else
					comp[i].node2 = Node;

				// finding intersecting comp. by QuadTree
				searchArea = { x - rectSize / 2.0f, y - rectSize / 2.0f, rectSize, rectSize };
				qtExtract(searchArea, intersects);

				for (int j = 0; j < intersects.size(); j++)
				{
					if (intersects[j] == i) continue;

					tempVec2.x = comp[intersects[j]].x;
					tempVec2.y = comp[intersects[j]].y;

					if ((sf::Vector2i)allEnds.back() == (sf::Vector2i)tempVec2) // Try Front End
					{
						comp[intersects[j]].node1 = Node;

						//auto it = std::find(end1.begin(), end1.end(), intersects[j]);
						//if (it != end1.end())
						//	end1.erase(it);
						end1[intersects[j]] = true;
					}
					else if ((sf::Vector2i)allEnds.back() == (sf::Vector2i)comp[intersects[j]].getEndPos()) // Try Back End
					{
						comp[intersects[j]].node2 = Node;

						//auto it = std::find(end2.begin(), end2.end(), intersects[j]);
						//if (it != end2.end())
						//	end2.erase(it);
						end2[intersects[j]] = true;
					}

				}


				// current's Front End
				{
					//auto it = std::find((*vec).begin(), (*vec).end(), Index);
					//if (it != (*vec).end()) {
					//	(*vec).erase(it);
					//	i--;
					//}
					(*vec)[i] = true;
				}

			}
		}

	}
	void updateAllEndsbyWires()
	{
		LOG("\n\nupdateAllEndsbyWires:");

		enum Rotation {
			None = 0,
			Vertical,
			Horizontal
		};
		int c = 0;
		for (auto& wire : wires) {

			//int index = -1; // Index in endNodes vector  |  -1 means invalid node index
			std::vector<int> compsOnWire; // Indices of vector: comp - sorted and unique
			std::vector<int> connectedEndNode; // Indices of vector: allEnds - sorted and unique

			if (wire.edge_points.size() < 2) {
				LOG("[X] Error: " << wire.edge_points.size() << "<2 points");
				continue;
			}

			for (unsigned long i = 0; i + 1 < wire.edge_points.size(); )
			{
				sf::Vector2f edge1 = wire.edge_points[i];
				sf::Vector2f edge2 = wire.edge_points[++i];

				Rotation rotation = Vertical;
				sf::FloatRect searchArea;
				std::vector<int> vec;

				if (edge1.x == edge2.x)
					rotation = Vertical;
				else if (edge1.y == edge2.y)
					rotation = Horizontal;

				// SearchArea
				{
					int margin = 6;

					if (rotation == Horizontal)
					{
						searchArea.top = edge1.y - margin;
						searchArea.left = (edge1.x < edge2.x) ? edge1.x - margin : edge2.x - margin;

						searchArea.height = 2 * margin;
						searchArea.width = std::abs(edge1.x - edge2.x) + 2 * margin;
					}
					else if (rotation == Vertical)
					{
						searchArea.top = (edge1.y < edge2.y) ? edge1.y - margin : edge2.y - margin;
						searchArea.left = edge1.x - margin;

						searchArea.width = 2 * margin;
						searchArea.height = std::abs(edge1.y - edge2.y) + 2 * margin;
					}
				}

				qtExtract(searchArea, vec);

				for (int i : vec)
				{
					if (rotation == Horizontal)
					{
						if (edge1.y == comp[i].y)
						{
							addToVector(i, compsOnWire);
							addToVector(comp[i].node1, connectedEndNode);
							comp[i].node1 = connectedEndNode.front();
						}
						else if (edge1.y == comp[i].getEndPos().y)
						{
							addToVector(i, compsOnWire);
							addToVector(comp[i].node2, connectedEndNode);
							comp[i].node2 = connectedEndNode.front();
						}

						/*if ((edge1.y == comp[i].y) ||
							(edge1.y == comp[i].getEndPos().y)) {
							addToVector(i, compsOnWire);
							addToVector(comp[i].node1, connectedEndNode);

							if (connectedEndNode.empty() == false)
								comp[i].node1 = connectedEndNode.front();
						}*/
					}
					else if (rotation == Vertical)
					{
						if (edge1.x == comp[i].x)
						{
							addToVector(i, compsOnWire);
							addToVector(comp[i].node1, connectedEndNode);
							comp[i].node1 = connectedEndNode.front();
						}
						else if (edge1.x == comp[i].getEndPos().x)
						{
							addToVector(i, compsOnWire);
							addToVector(comp[i].node2, connectedEndNode);
							comp[i].node2 = connectedEndNode.front();
						}

						/*if ((edge1.x == comp[i].x) ||
							(edge1.x == comp[i].getEndPos().x)) {
							addToVector(i, compsOnWire);
							addToVector(comp[i].node2, connectedEndNode);

							if (connectedEndNode.empty() == false)
								comp[i].node2 = connectedEndNode.front();
						}*/
					}
				}

			}

			
			{
				//erase - remove idiom
				//auto iter = std::remove_if(allEnds.begin(), allEnds.end(),
				//	[&](const auto& elem) {
				//		return std::binary_search(connectedEndNode.begin() + 1, connectedEndNode.end(), &elem - &allEnds[0]);
				//	}
				//);
				//allEnds.erase(iter, allEnds.end());
			}
			
			//updateVisibleVectors();



			//LOG("\ncompsOnWire:"); LOG_VEC(compsOnWire);
			LOG(++c << " connectedEndNode:"); LOG_VEC(connectedEndNode);

		}
	}
	bool makingWire() {
		if (wires.empty())
			return false;
		else if (wires.back().isStopped())
			return false;
		else
			return true;
	}

	void EndLessPit() {
		for (long long i = 0; i < 9999; i++)
			LOG(" Falling");
	}

	static bool qtChangeTag = true;
	bool visible_QuadTree = true;
	struct quadTree {
		bool changed = false;
		bool isSubDivided = false;
		static const int sizeLimit = 5; // Ignored if the boxSize reaches boxSizeLimit
		static const int boxSizeLimit = 5 * gap;
		//unsigned int size = 0;
		std::vector<int> arr; // Sorted Indices
		sf::FloatRect bounds; // Bounding Box for Calculation
		sf::RectangleShape rectDraw; // For Drawing only

		// [0 1]
		// [2 3]
		quadTree* sub[4] = { nullptr, nullptr ,nullptr ,nullptr };

		quadTree() {
			arr.reserve(quadTree::sizeLimit);
		}
	};
	quadTree qt;
	void qtClean(quadTree& box) {

		;

		//if (box.isSubDivided) {
		//	qtClean(*box.sub[0]);
		//	qtClean(*box.sub[1]);
		//	qtClean(*box.sub[2]);
		//	qtClean(*box.sub[3]);
		//
		//	//if (box.sub[0] == nullptr && box.sub[1] == nullptr &&
		//	//	box.sub[2] == nullptr && box.sub[3] == nullptr)
		//	if (box.sub[0]->arr.empty() && box.sub[1]->arr.empty() &&
		//		box.sub[2]->arr.empty() && box.sub[3]->arr.empty())
		//	{
		//		//LOG("\nqtClean: all subs nulptr");
		//		qtDelete(box);
		//	}
		//
		//}
		//else if (box.arr.empty())
		//{
		//	//LOG("\nqtClean: box.arr.empty, so qtDeleting");
		//	qtDelete(box);
		//}

	}
	void qtRemove(int c, quadTree& box) {

		if (box.bounds.intersects(comp[c].bounds))
		{
			if (box.isSubDivided) {
				qtRemove(c, *box.sub[0]);
				qtRemove(c, *box.sub[1]);
				qtRemove(c, *box.sub[2]);
				qtRemove(c, *box.sub[3]);
			}
			else {
				auto it = std::lower_bound(box.arr.begin(), box.arr.end(), c);
				
				if (it != box.arr.end())
				{
					if (it - box.arr.begin() <= box.arr.size() - !box.arr.empty() && box.arr[it - box.arr.begin()] == c)
					{
						//qtWrite();
						box.arr.erase(it);
						
						//qtClean();

					}
				}
			}

		}

	}
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
		}

		box.isSubDivided = false;
	}
	void qtAdd(int c, quadTree& box) {
		if (box.bounds.intersects(comp[c].bounds)) {
			if (box.isSubDivided == false && (
				(box.arr.size() < quadTree::sizeLimit) ||
				(std::min(box.bounds.width, box.bounds.height) < quadTree::boxSizeLimit))) {
				box.arr.emplace_back(c);
				//box.size++; // = box.arr.size();
			}
			else {

				if (box.isSubDivided == false) { // HardCode (5)
					
					// Sub-divide
					{
						box.isSubDivided = true;
						int qtHalfWidth = (int)(box.bounds.width / 2);
						int qtHalfHeight = (int)(box.bounds.height / 2);

						for (int i = 0; i < 4; i++) {
							box.sub[i] = new quadTree;

							box.sub[i]->bounds.width = qtHalfWidth;
							box.sub[i]->bounds.height = qtHalfHeight;
							box.sub[i]->bounds.left = box.bounds.left;
							box.sub[i]->bounds.top = box.bounds.top;
						}
						box.sub[1]->bounds.left += qtHalfWidth;
						box.sub[2]->bounds.top += qtHalfHeight;
						box.sub[3]->bounds.left += qtHalfWidth;
						box.sub[3]->bounds.top += qtHalfHeight;


						for (int i = 0; i < 4; i++) {
							box.sub[i]->rectDraw.setSize(sf::Vector2f(box.sub[i]->bounds.width, box.sub[i]->bounds.height));
							box.sub[i]->rectDraw.setPosition(sf::Vector2f(box.sub[i]->bounds.left, box.sub[i]->bounds.top));
							box.sub[i]->rectDraw.setFillColor(sf::Color(
								box.sub[i]->rectDraw.getPosition().y - box.sub[i]->rectDraw.getPosition().x,
								box.sub[i]->rectDraw.getPosition().x + box.sub[i]->rectDraw.getLocalBounds().height,
								box.sub[i]->rectDraw.getLocalBounds().width - box.sub[i]->rectDraw.getPosition().x, 100));

							//box.sub[i]->rectDraw.setOutlineColor(sf::Color::Blue);
							//box.sub[i]->rectDraw.setOutlineThickness(10);
						}

					}

					// Add qtArr in subdivided areas
					{
						for (int i : box.arr)
						{
							qtAdd(i, *box.sub[0]);
							qtAdd(i, *box.sub[1]);
							qtAdd(i, *box.sub[2]);
							qtAdd(i, *box.sub[3]);
						}
						box.arr.clear();
						//LOG("\nbox.arr.clear()");
					}

				}

				qtAdd(c, *box.sub[0]);
				qtAdd(c, *box.sub[1]);
				qtAdd(c, *box.sub[2]);
				qtAdd(c, *box.sub[3]);
				//box.size++;
			}

			//return 1;
		}
		else {
			if (&box == &qt)
			{
				//LOG("qt update");
				qtUpdate();
			}
		}
	}
	void qtUpdate() {

		qtDelete(qt);
		qt.bounds = areaofCollection(true);

		// rectDraw
		{
			qt.rectDraw.setSize(sf::Vector2f(qt.bounds.width, qt.bounds.height));
			qt.rectDraw.setPosition(sf::Vector2f(qt.bounds.left, qt.bounds.top));
			qt.rectDraw.setFillColor(sf::Color(
				qt.rectDraw.getPosition().y - qt.rectDraw.getPosition().x,
				qt.rectDraw.getPosition().x + qt.rectDraw.getLocalBounds().height,
				qt.rectDraw.getLocalBounds().width - qt.rectDraw.getPosition().x, 100));
		}

		int compSize = comp.size();
		if (compSize <= quadTree::sizeLimit) {
			//qt.size = compSize;
			//LOG("\nqt: " << qt.arr.size() << "\tcomps: " << compSize);
			qt.arr.resize(compSize); // no del
			//LOG("\nqt: " << qt.arr.size() << "\tcomps: " << compSize << "\n");
			for (int c = 0; c < compSize; c++) // qt.arr.size()
				qt.arr[c] = c;
		}
		else {
			for (int c = 0; c < compSize; c++)
				qtAdd(c, qt);
		}
	}
	void qtWrite(const quadTree& box, int indentation) {

		for (int t = 0; t < indentation; t++) LOG("\t");
		if (box.isSubDivided) LOG("`");
		//LOG("(" << box.size << "): ");
		LOG("(" << box.arr.size() << "): ");
		for (int i = 0; i < box.arr.size(); i++)
			LOG(box.arr[i] << ", ");
		LOG("\b\b \n");

		if (box.isSubDivided) {
			//for (int i = 0; i < 4; i++)
			//	LOG("\nsub[" << i << "] = " << box.sub[i]);

			qtWrite(*box.sub[0], indentation + 1);
			qtWrite(*box.sub[1], indentation + 1);
			qtWrite(*box.sub[2], indentation + 1);
			qtWrite(*box.sub[3], indentation + 1);
		}

	}
	void qtDrawZ(quadTree& box) {

		if (visible_QuadTree == false)
			return;

		static sf::FloatRect searchArea;

		if (&box == &qt)
			searchArea = { sf::Vector2f(
				view.getCenter().x - view.getSize().x / 2,
				view.getCenter().y - view.getSize().y / 2), view.getSize()
		};

		if (box.bounds.intersects(searchArea))
			app.draw(box.rectDraw);
		else return;

		if (box.isSubDivided) {
			qtDraw(*box.sub[0]);
			qtDraw(*box.sub[1]);
			qtDraw(*box.sub[2]);
			qtDraw(*box.sub[3]);
		}

	}
	void qtDraw(quadTree& box) {

		if (visible_QuadTree == false)
			return;

		static sf::FloatRect searchArea;

		if (&box == &qt)
			searchArea = { sf::Vector2f(
				view.getCenter().x - view.getSize().x / 2,
				view.getCenter().y - view.getSize().y / 2), view.getSize()
		};

		if (box.bounds.intersects(searchArea))
		{
			if (box.isSubDivided) {
				qtDraw(*box.sub[0]);
				qtDraw(*box.sub[1]);
				qtDraw(*box.sub[2]);
				qtDraw(*box.sub[3]);
			}
			else app.draw(box.rectDraw);
		}
		else return;
	}
	void qtExtract(const sf::FloatRect& searchArea, std::vector<int>& output, const quadTree& box, bool clearVector) {

		if (&box == &qt && clearVector)
			output.clear();

		if (searchArea.intersects(box.bounds)) {

			if (box.isSubDivided == false)
			{
				// Check if box resides entirly in searchArea
				// If Yes, then add all the elements of box.arr in the box
				if (searchArea.contains(box.bounds.left, box.bounds.top) &&
					searchArea.contains(box.bounds.left + box.bounds.width, box.bounds.top + box.bounds.height))
				{
					for (int i : box.arr)
						addToVector(i, output);
				}
				else
				{
					for (int i : box.arr)
						if (searchArea.intersects(comp[i].bounds))
							addToVector(i, output);
				}
			}
			else {
				qtExtract(searchArea, output, *box.sub[0]);
				qtExtract(searchArea, output, *box.sub[1]);
				qtExtract(searchArea, output, *box.sub[2]);
				qtExtract(searchArea, output, *box.sub[3]);
			}
		}

	}

	bool occupiedAt(const Entity& en, const sf::Vector2f& At, bool ignoreAllVir) {

		static Entity e;
		e.x = At.x; e.y = At.y; e.angle = en.angle;
		e.stimuli();

		std::vector<int> vec;
		qtExtract(e.bounds, vec);

		if (ignoreAllVir)
		{
			for (int v = virSerial.size() - 1; v >= 0; v--)
			{
				int index = lower_bound(vec.begin(), vec.end(), virSerial[v]) - vec.begin();
				
				if (index < vec.size() && (vec[index] == virSerial[v]))
					vec.erase(vec.begin() + index);
			}
		}

		int count = 0;
		for (int i = 0; i < vec.size(); i++) {
			count = 0;
			static sf::Vector2f eEnd(0, 0);
			static sf::Vector2f cEnd(0, 0);

			eEnd = e.getEndPos();
			cEnd = comp[vec[i]].getEndPos();

			if (e.x == comp[vec[i]].x &&
				e.y == comp[vec[i]].y)
				count++;

			if (e.x == cEnd.x &&
				e.y == cEnd.y)
				count++;

			if (eEnd.x == comp[vec[i]].x &&
				eEnd.y == comp[vec[i]].y)
				count++;

			if (eEnd.x == cEnd.x &&
				eEnd.y == cEnd.y)
				count++;

			if (count == 0) { Occupied = true; return 1; }
			if (count > 1)  { Occupied = true; return 1; }
		}

		if (count == 1) { Occupied = false; return 0; }

		Occupied = (!vec.empty());
		return (!vec.empty());

	}
	sf::FloatRect areaofCollection(int collection, bool* sucess_ptr) {
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
	void updateVisibleVectors()
	{
		// visibleComps
		{
			sf::FloatRect searcharea(sf::Vector2f(
				view.getCenter().x - view.getSize().x / 2,
				view.getCenter().y - view.getSize().y / 2), view.getSize());
			qtExtract(searcharea, visibleComps);
		}

		// visibleBoarders
		{
			visibleBoarders.clear();

			for (int i : visibleComps)
				if (std::binary_search(virSerial.begin(), virSerial.end(), i))
					visibleBoarders.emplace_back(i);
		}
		
		// visibleEndNodes
		{
			visibleEndNodes.clear();

			for (int i : visibleComps) {
				addToVector(comp[i].node1, visibleEndNodes);
				addToVector(comp[i].node2, visibleEndNodes);
			}
		}

	}
	void addToVector(int integer, std::vector<int>& vec)
	{
		//if (std::binary_search(vec.begin(), vec.end(), integer) == false)
		{
			auto it = std::lower_bound(vec.begin(), vec.end(), integer);
			if (it == vec.end()) // Not Found
				vec.emplace_back(integer);
			else {
				//if (vec[it - vec.begin() + 1] != integer)
				//vec.emplace(it, integer);

				int insert_pos = it - vec.begin();
				if (insert_pos <= vec.size() - !vec.empty() && vec[insert_pos] != integer)
					vec.insert(vec.begin() + insert_pos, integer);
			}
		}
	}

	void colorEntityboarder() {

		sf::Color temp;

		if (darkLightMode)
			temp = { 0, 255, 85 };
		else
			temp = { 0/*200*/, 204, 102 }; // 44, 116, 97, 200

		Entity::setboarderDesgin(temp);

		for (auto& entity : comp)
			entity.boarder.setOutlineColor(temp);

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
		if (virSerial.size() > 1)
			app.draw(allSqr);
	}


	// ToolBox
	const float c_toolColWidth = 100;
	sf::Vector2f toolWinRestPos(0, 0);
	sf::RectangleShape toolCol(sf::Vector2f(c_toolColWidth, CircuitGUI::view.getSize().y));
	sf::RectangleShape ToolBoxLittleBox(sf::Vector2f(c_toolColWidth, c_toolColWidth));
	//void toolBoxInit()
	sf::Sprite ToolSpr[Entity::no_of_Comp - 1];
	sf::Vector2f ToolSprPOS[Entity::no_of_Comp - 1];
	void updatePosToolBox()
	{
		toolWinRestPos = onScreen(0, 0);
		for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
			ToolSpr[c].setPosition(onScreen(ToolSprPOS[c].x, ToolSprPOS[c].y));
	}
	void drawToolColumn(bool MInTool, bool MIntool) {
		if (MInTool) {
			app.draw(toolCol);

			for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
				ToolSpr[c].setColor(sf::Color(255, 255, 255, ToolSpr[c].getColor().a + (255 - ToolSpr[c].getColor().a) / 15));

			for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
				app.draw(ToolSpr[c]);
		}
		else {
			for (int c = 0; c < (Entity::no_of_Comp - 1); c++)
				ToolSpr[c].setColor(sf::Color::Transparent);
		}

		if (MIntool) app.draw(ToolBoxLittleBox);
	}


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
		//initializeRenderWindow();
		{
			W = sf::VideoMode::getDesktopMode().width * 0.85;
			H = W * 9 / 16;
			if (H < c_toolColWidth * 7) H = c_toolColWidth * 7;

			app.create(sf::VideoMode((unsigned int)W, (unsigned int)H), "CircuitSim", sf::Style::Default, sf::ContextSettings(0, 0, 8));
			app.clear(backColor);
			app.display();
			W = app.getSize().x; H = app.getSize().y;
			
			app.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - W / 2, sf::VideoMode::getDesktopMode().height / 2 - H / 2 - 50));
			app.setKeyRepeatEnabled(false);
			app.setVerticalSyncEnabled(true);
			

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

		//themeColors
		updateThemeColors();
		
		//setFont
		{
			//ResourceID_Fonts.SetAll(IDR_FONT1, "FONT");
			//Entity::setFont(ResourceID_Fonts.GetResource().ptr, ResourceID_Fonts.GetResource().size_bytes);
			Entity::setFont("assets/Fonts/CalibriL_1.ttf");

			//ResourceID_Fonts.SetAll(IDR_FONT1, "FONT");
			//Item::setFont(ResourceID_Fonts.GetResource().ptr, ResourceID_Fonts.GetResource().size_bytes);
			Item::setFont("assets/Fonts/CalibriL_1.ttf");
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

		// Vectors
		{
			comp.reserve(15);
			allEnds.reserve(30);
			virSerial.reserve(15);
			virSprite.reserve(15);
			virSerialShift.reserve(5);
			wires.reserve(8);
			visibleComps.reserve(20);
			visibleEndNodes.reserve(20);
			visibleBoarders.reserve(20);

			//newComps.reserve(15);
			//newItems.reserve(5);
		}

		selSqrDesign();

		allSqrDesign();
	}



	namespace Options
	{
		void Open(const std::string& filepath) {

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
			updateVisibleVectors();
		}

		void Save(const std::string& file) {

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

		void SaveAsImage(const std::string& filepath) {

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
					{
						Bounds_of_all_Entities.width += Bounds_of_all_Entities.left - boundWire.left; Bounds_of_all_Entities.left = boundWire.left;
					}
					if (boundWire.top < Bounds_of_all_Entities.top)
					{
						Bounds_of_all_Entities.height += Bounds_of_all_Entities.top - boundWire.top; Bounds_of_all_Entities.top = boundWire.top;
					}
					if (boundWire.left + boundWire.width > Bounds_of_all_Entities.left + Bounds_of_all_Entities.width)
						Bounds_of_all_Entities.width = boundWire.left - Bounds_of_all_Entities.left + boundWire.width;
					if (boundWire.top + boundWire.height > Bounds_of_all_Entities.top + Bounds_of_all_Entities.height)
						Bounds_of_all_Entities.height = boundWire.top - Bounds_of_all_Entities.top + boundWire.height;
				}
			}


			if (comp.empty() && wires.empty())
				image.create(10, 10, backColor);
			else
				image.create(Bounds_of_all_Entities.width + 2 * gap, Bounds_of_all_Entities.height + 2 * gap, backColor);


			// Comps
			for (auto& en : comp) {

				int x_offset = gap + en.bounds.left - Bounds_of_all_Entities.left;
				int y_offset = gap + en.bounds.top - Bounds_of_all_Entities.top;

				//sf::Texture tex(*en.sprite.getTexture());
				static sf::Texture tex;
				if (en.sprite.getTexture() == nullptr)
				{
					tex.create(compTex.back().getSize().x, compTex.back().getSize().y);
					tex.update(compTex.back());
				}
				else
				{
					tex.create(en.sprite.getTexture()->getSize().x, en.sprite.getTexture()->getSize().y);
					tex.update(*en.sprite.getTexture());
				}

				static sf::Image img;

				switch ((int)en.angle) {
				case 0:
				{
					//std::cout << "\n"<<(int)(en.sprite.getTexture()->getSize().x/2.0f - gap);
					x_offset -= (int)(tex.getSize().x / 2.0f - gap);
					img = tex.copyToImage();
					break;
				}
				case 180:
				{
					x_offset -= (int)(tex.getSize().x / 2.0f - gap);
					y_offset -= (int)(tex.getSize().y - gap * 5);

					img = (tex.copyToImage());
					img.flipVertically();
					img.flipHorizontally();
					break;
				}
				case 90:
				case 270:
				{
					static sf::Image rot_img;
					rot_img = tex.copyToImage();
					img.create(rot_img.getSize().y, rot_img.getSize().x);

					for (int j = 0; j < rot_img.getSize().y; j++)
						for (int i = 0; i < rot_img.getSize().x; i++)
							img.setPixel(j, i, rot_img.getPixel(i, j));
					img.flipHorizontally();

					if (en.angle == 90) {
						x_offset -= (int)(tex.getSize().y - gap * 5);
						y_offset -= (int)(tex.getSize().x / 2.0f - gap);
					}
					else if (en.angle == 270) {
						y_offset -= (int)(tex.getSize().x / 2.0f - gap);

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
			for (auto& wire : wires) {

				bool sucess = false;
				sf::FloatRect bound(wire.bounds(&sucess));

				if (sucess == false || wire.noOfDrawRects() < 1)
					continue;

				auto& vec = *wire.getRectVector();


				for (auto& rec : vec) {

					int x_offset = gap + rec.getGlobalBounds().left - Bounds_of_all_Entities.left;
					int y_offset = gap + rec.getGlobalBounds().top - Bounds_of_all_Entities.top;

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
		
		void Copy() {

			std::ofstream output;
			std::string tempStr(std::to_string((int)virSerial.size()) + "\n");

			// Components
			{
				for (int i : virSerial)
					tempStr += std::to_string(comp[i].getSerial()) + "\t" + std::to_string((int)comp[i].x) + "\t" + std::to_string((int)comp[i].y) + "\t" + std::to_string((int)comp[i].angle) + "\n";
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

		void Paste() {

			std::vector<int> integers; integers.reserve(21);
			std::string inString(sf::Clipboard::getString());
			
			size_t wire_start = inString.find_first_of('#');

			std::string comp_str(inString.substr(0, wire_start));
			std::string wire_str(inString.substr((wire_start + 8), (inString.size() - wire_start))); // HardCode (7) length("#Wires:\n")
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
				if (integers.empty() == false) {

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


					qtUpdate();
					updateVisibleVectors();
				}
			}

			// Wires
			{
				//LOG("\nwire paste:\n");
				sf::Vector2f offSet(OffsetX, OffsetY);
				size_t wire_stop;
				std::string sub;
				wire_str += "\n";
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
					// TODO: Doesn't Work when \n is missing at the end
					else LOG("\n[Warrning] Wire sub.size() > 3 | sub = \"" << sub << "\" | Options::Paste()");
					wire_str.erase(wire_str.begin(), wire_str.begin() + (wire_stop + 1));
				}

			}

		}

		void Rotate(bool RotateAll) {

			sf::FloatRect bounds = areaofCollection(0);
			sf::Vector2f origin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
			origin = trim(origin);
			origin.x = (int)origin.x;
			origin.y = (int)origin.y;

			for (int v : virSerial)
			{
				comp[v].angle += 90;
				comp[v].angle -= (int)(comp[v].angle / 360) * 360;
				comp[v].stimuli();

				if (RotateAll)
				{
					sf::Vector2f pos(comp[v].x, comp[v].y);

					comp[v].x = origin.x + origin.y - pos.y;
					comp[v].y = origin.y + pos.x - origin.x;
				}

				comp[v].stimuli();
			}

			if (virSerial.empty() == false) {
				qtUpdate();
				updateVisibleVectors();
			}
		}

		void Delete() {

			//erase - remove idiom
			if (virSerial.empty() == false)
			{
				auto iter = std::remove_if(comp.begin(), comp.end(),
					[&](const auto& elem) {
						return std::binary_search(virSerial.begin(), virSerial.end(), &elem - &comp[0]);
					}
				);

				comp.erase(iter, comp.end());
			}

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

			qtUpdate();
			updateVisibleVectors();
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