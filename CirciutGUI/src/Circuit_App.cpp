
/*
	* Colapse all Scopes - it'll be easier
	* Ctrl + M + A in Visual Studio
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <direct.h>
//#include <sstream>
//#include <string>
//#include <thread>
//#include <future>

#include "LOG.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "Circuit_GUI.hpp"
#include "Circuit_Entity.hpp"
#include "Circuit_Windows_Stuff.hpp"
#include "Resource_Manager.hpp"
#include "Circuit_Wire.hpp"
#include "Circuit_Item.hpp"
#include "Circuit_Graph.hpp"
#include "Circuit_App.hpp"
//#include "CircuitCore.hpp"

App::App(const std::vector<std::string>& filepaths)
{
	srand(time(NULL));
	
	CircuitGUI::initializeGUI();

	/*ImGui*/
	ImGui::SFML::Init(CircuitGUI::app);

	time_t FrameTime_for_FPS = clock();


	Resource my_pic(IDR_CAP, "BMP");
	Resource my_font(IDR_FONT2, "FONT");
	//my_font.GetInfo();

	/* Test Circle
	bool DrawCircle = 1; int t_vertices = 34; float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 }; sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/


#ifdef _DEBUG
	// For Opening Where Left
	CircuitGUI::Options::Open("temp_files/last.txt");
	stimuliDisplay = 1; stimuliEndNodes = 1;
#endif

	////////////////// argv ///////////////////////
	if (filepaths.empty() == false)
	{
		CircuitGUI::Options::Open(filepaths.back());

		stimuliEndNodes = 1; stimuliDisplay = 1; //CircuitGUI::updateAllSqr();
	}
}

App::~App()
{
	//ImGui
	ImGui::SFML::Shutdown();

#ifdef _DEBUG
	CircuitGUI::Options::Save("temp_files/last.txt");

	std::cin.get();
#else
	//LOG::log_file.close();
#endif

}

void App::Run()
{
	// Solver
	Graph circuit;

	//////////////// Main Loop ////////////////////
	while (CircuitGUI::app.isOpen() && End == false)
	{
		Events();
		
		Options();

		ImGUI();

		Update();

		Render();

		EndFrame();
	}

}



void App::Events()
{
	while (CircuitGUI::app.pollEvent(CircuitGUI::evnt)) {
		using namespace CircuitGUI;
		stimuliDisplay = 1; /*cout << "1";*/
		/*ImGui*/
		ImGui::SFML::ProcessEvent(evnt);

		if (evnt.type == evnt.Closed) {

			std::ofstream window_size("temp_files/win_size.txt");
			window_size << CircuitGUI::app.getSize().x << "\t";
			window_size << CircuitGUI::app.getSize().y << "\t";
			window_size << CircuitGUI::app.getPosition().x << "\t";
			window_size << CircuitGUI::app.getPosition().y << "\t";
			window_size.close();

			app.close(); End = 1;
		}

		if (evnt.type == evnt.MouseLeft) { cursorInWin = 0; }

		if (evnt.type == evnt.MouseEntered) { cursorInWin = 1; }

		if (evnt.type == evnt.Resized) {
			W = CircuitGUI::app.getSize().x; H = CircuitGUI::app.getSize().y;

			view = sf::View(sf::FloatRect((int)view.getCenter().x - (int)(evnt.size.width / 2), (int)view.getCenter().y - (int)(evnt.size.height / 2), (int)evnt.size.width, (int)evnt.size.height));
			//CircuitGUI::view.setSize((int)evnt.size.width, (int)evnt.size.height);
			//CircuitGUI::view.setCenter((int)CircuitGUI::view.getCenter().x, (int)CircuitGUI::view.getCenter().y);

			CircuitGUI::toolCol.setSize(sf::Vector2f(CircuitGUI::toolCol.getSize().x, CircuitGUI::view.getSize().y));
			CircuitGUI::updatePosToolBox();


			std::ofstream window_size("temp_files/win_size.txt");
			window_size << CircuitGUI::app.getSize().x << "\t";
			window_size << CircuitGUI::app.getSize().y << "\t";
			window_size << CircuitGUI::app.getPosition().x << "\t";
			window_size << CircuitGUI::app.getPosition().y << "\t";
			window_size.close();

			updateVisibleVector();
		}

		if (evnt.type == evnt.MouseButtonPressed) {
			if (evnt.mouseButton.button == sf::Mouse::Left) {
				using namespace CircuitGUI;

				static sf::Vector2f cursor_position;
				cursor_position = cursorInSim();

				static std::vector<int> vec;
				static sf::FloatRect searchArea; {
					static const float box_width = gap;

					searchArea.left = cursor_position.x - box_width / 2.0f; // HardCode (3)
					searchArea.top = cursor_position.y - box_width / 2.0f;
					searchArea.width = box_width;
					searchArea.height = box_width;
				}

				qtExtract(searchArea, vec);

				bool on_Node = false;
				static int sensitivity = 7; // HardCode
				static sf::Vector2f node_position;

				for (int v : vec) { // v is vec[i]
					if (std::abs(comp[v].x - cursor_position.x) < sensitivity &&
						std::abs(comp[v].y - cursor_position.y) < sensitivity)
					{
						//LOG("\nOnNode");
						on_Node = true;
						node_position = sf::Vector2f(comp[v].x, comp[v].y);
						break;
					}
					else
					{
						static sf::Vector2f end;
						end = comp[v].getEndPos();

						if (
							std::abs(end.x - cursor_position.x) < sensitivity &&
							std::abs(end.y - cursor_position.y) < sensitivity)
						{
							//LOG("\nOnNode");
							on_Node = true;
							node_position = end;
							break;
						}
					}
				}

				if (on_Node)
				{
					if (wires.empty())							// No Wires:
						wires.emplace_back(node_position);		// So, starting a new wire
					else {
						if (wires.back().isStopped() == false) {// Wire is being build:
							wires.back().newEdge();			// So, Inserting Last point
							wires.back().stop();				// and then stopping a current wire
						}
						else
							wires.emplace_back(node_position);	// No Wire being build: So, starting a new wire
					}
				}
				else if (wires.empty() == false)
					if (wires.back().isStopped() == false)
						wires.back().newEdge();

			}
			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				Drag = true;
				mouseOnCompsBool = 0;
				CircuitGUI::iniDrag();
			}
		}

		if (evnt.type == evnt.MouseButtonReleased) {
			if (evnt.mouseButton.button == sf::Mouse::Left) {
				releaseBool = 1;
				if (Selection) {
					Selection = 0;

					//virSprite.clear();
					virSprite.reserve(virSerial.size());
					/*for (int v = 0; v < virSerial.size(); v++) {
						virSprite.emplace_back(comp[virSerial[v]].sprite);
						virSprite.back().setColor(CircuitGUI::tempDimColor); }*/


					for (int v = 0; v < virSerial.size() && v < virSprite.size(); v++) {
						virSprite[v] = comp[virSerial[v]].sprite;
						virSprite[v].setColor(CircuitGUI::tempDimColor);
					}

					if (virSerial.size() < virSprite.size())
					{
						virSprite.erase(virSprite.begin() + (int)virSerial.size(), virSprite.end());
					}

					if (virSerial.size() > virSprite.size())
					{
						for (int v = virSprite.size(); v < virSerial.size(); v++) {
							virSprite.emplace_back(comp[virSerial[v]].sprite);
							virSprite.back().setColor(CircuitGUI::tempDimColor);
						}

					}

				}
			}
			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				Drag = false;
			}
		}

		if (evnt.type == evnt.KeyPressed) {
			if (evnt.key.code == sf::Keyboard::Escape) { app.close(); End = 1; continue; }
			if (evnt.key.code == sf::Keyboard::Delete) {
				stimuliDisplay = 1; /*cout << "2";*/	stimuliEndNodes = 1;
				CircuitGUI::Options::Delete();
			}
			if (evnt.key.code == sf::Keyboard::L) {

				LOG("\n\nwires (" << wires.size() << "): ");
				for (auto& w : wires)
					LOG(w.noOfEdges() << " ");

				LOG("\ncursor: "); LOG_VEC2(cursorInSim());

				LOG("\n");
			}
			if (evnt.key.code == sf::Keyboard::E) {





				//comp.emplace_back(Item(rand() % (Item::Item_Type::no_of_Comp + 1), trim(rand() % W), trim(rand() % H), 90 * (rand() % 4)));
				//LOG("\ncomp size: " << comp.size());
				//qtUpdate();
				//updateVisibleVector();


				//LOG("----------\n\n\n" << component.node1 << ", " << component.node2<<"\n");
				//comp.emplace_back(Item(rand() % (Item::Item_Type::no_of_Comp + 1), trim(rand() % W), trim(rand() % H), 90 * (rand() % 4)));

				//newItems.emplace_back(rand() % (Item::Item_Type::no_of_Comp + 1), trim(rand() % W), trim(rand() % H), 90 * (rand() % 4));

				//newComps.emplace_back(static_cast<std::vector<Entity>::iterator>(newItems.end()));
				//auto att = newComps.back() - newItems.begin() - !newItems.empty();
				//auto i_ = &newItems.at(att);
				//LOG("\n\nptr : " << i_ << "\tValue: " << *i_);
				//LOG("\n_Ptr: " << newComps.back()._Ptr-- /*<< "\tValue: " << (newComps.back()._Ptr--)->getSerial()*/);
				//newComps.emplace_back(newItems.end());
				//LOG("\nnewComps size: " << newComps.size());
			}

			// Ctrl
			if (evnt.key.control) {
				if (evnt.key.code == sf::Keyboard::A) {
					stimuliDisplay = 1; /*cout << "3";*/

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
					stimuliDisplay = 1;	/*cout << "4";*/ stimuliEndNodes = 1;

					CircuitGUI::Options::Rotate();
				}
				if (evnt.key.code == sf::Keyboard::C) {

					CircuitGUI::Options::Copy();
					CircuitGUI::qtUpdate();
				}
				if (evnt.key.code == sf::Keyboard::X) {
					stimuliDisplay = 1;	/*cout << "6";*/ stimuliEndNodes = 1;

					// Copy
					{
						CircuitGUI::Options::Copy();
					}

					// Delete
					{
						CircuitGUI::Options::Delete();
					}

				}
				if (evnt.key.code == sf::Keyboard::V) {
					stimuliDisplay = 1; /*cout << "7";*/ stimuliEndNodes = 1;

					CircuitGUI::Options::Paste();
				}
				if (evnt.key.code == sf::Keyboard::O) {
					stimuliDisplay = 1; /*cout << "5";*/ stimuliEndNodes = 1;

					std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

					if (!filepath.empty())
						CircuitGUI::Options::Open(filepath);
				}
				if (evnt.key.code == sf::Keyboard::S) {
					;
					//_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
					//_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
				}
				if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {

					_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
					_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
					std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

					if (!filepath.empty()) {

						if (filepath.back() == 'T')
							CircuitGUI::Options::Save(filepath);

						if (filepath.back() == 'G')
							CircuitGUI::Options::SaveAsImage(filepath);

					}

				}
			}

			//if (evnt.key.code == sf::Keyboard::W) { wireBool = !wireBool; }
			//if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool;}

			/*if (evnt.key.code == sf::Keyboard::K) {
				LOG("\nCurrent Frame");
				sf::Texture texture;
				texture.create(app.getSize().x, app.getSize().y);
				texture.update(app, -40, -50);

				static int count = 0;
				sf::Image screenshot = texture.copyToImage();
				screenshot.saveToFile("screenshot-" + std::to_string(count++) + ".png");
			}*/

			/*if (evnt.key.code == sf::Keyboard::K) {
				LOG("test img");
				sf::Texture texture;
				texture.create(75, 100);
				texture.update(compTex[Entity::Dod], 10, 10);

				sf::Image test = texture.copyToImage();
				test.saveToFile("test.png");
			}*/

		}

		{
			//if (evnt.type == evnt.MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left) {
			//	bool onNode = 0;
			//	static int sensitivity = 7;
			//	sf::Vector2f cursorPos = CircuitGUI::cursorInSim();

			//	sf::Vector2f start(0, 0), stop(0, 0);
			//	if (comp.size() > 2) start = { comp[0].x, comp[0].y }, stop = { comp[1].x, comp[1].y };


			//	int e = 0;
			//	/*asdfif (PlayMode) {
			//		sensitivity = 30;
			//		if (abs(cursorPos.x - start.x) < sensitivity && abs(cursorPos.y - start.y) < sensitivity) { onNode = 1; e = 1; }
			//		else if (abs(cursorPos.x - stop.x) < sensitivity && abs(cursorPos.y - stop.y) < sensitivity) { onNode = 1; e = 2; }
			//	}
			//	else*/ {
			//		for (; e < CircuitGUI::allEnds.size(); e++) {
			//			if (abs(cursorPos.x - CircuitGUI::allEnds[e].x) < sensitivity && abs(cursorPos.y - CircuitGUI::allEnds[e].y) < sensitivity) { onNode = 1; break; }
			//		}
			//	}

			//	if (onNode) {

			//		/*asdfif (PlayMode)
			//		{
			//			if (!wireBool) {
			//				if (e == 1) {
			//					PlayRot = 1; wires.emplace_back(start); wireBool = 1;
			//				}
			//			}
			//			else if (e == 2) { PlayRot = 0; PlayMode = 0; wireBool = 0; }

			//		}
			//		else*/
			//		{
			//			std::cout << ", On Node";

			//			if (!wireBool) {
			//				//asdfif (PlayMode) PlayRot = 1;
			//				wires.emplace_back(CircuitGUI::allEnds[e]);
			//			}

			//			wireBool = !wireBool;

			//		}


			//	}
			//	else {

			//		if (wireBool) {
			//			wires.back().newEdge();
			//		}

			//	}

			//}
		}

		/*int difr = 10;
		if (evnt.key.code == sf::Keyboard::Up)		{ CircuitGUI::view.move(0, -difr); }
		if (evnt.key.code == sf::Keyboard::Down)	{ CircuitGUI::view.move(0, difr); }
		if (evnt.key.code == sf::Keyboard::Right)	{ CircuitGUI::view.move(difr, 0); }
		if (evnt.key.code == sf::Keyboard::Left)	{ CircuitGUI::view.move(-difr, 0); }*/
	}

	ShiftPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
	MInTool = !!(cursorInWin && sf::Mouse::getPosition(CircuitGUI::app).x <= CircuitGUI::c_toolColWidth);
	MIntool = !!(cursorInWin && MInTool && sf::Mouse::getPosition(CircuitGUI::app).y < (Entity::no_of_Comp - 1) * CircuitGUI::c_toolColWidth);
	t_TollWx = CircuitGUI::toolCol.getPosition().x;
	// myWinState = CircuitGUI::Response::State::None;
}

void App::Options()
{
	//sf::Mouse Hold
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { //////////// Urgent need of enums , State Machine
		if (releaseBool) {
			using namespace CircuitGUI;
			releaseBool = 0;
			click = clock();
			mouseHoldX = (float)sf::Mouse::getPosition(app).x; mouseHoldY = (float)sf::Mouse::getPosition(app).y;

			/*new Comp*/
			if (MIntool) {
				MIntool = 0;
				float tempNewCompX = 150 + view.getCenter().x - W / 2, tempNewCompY = 150 + view.getCenter().y - H / 2;
				tempNewCompX = trim(tempNewCompX);
				tempNewCompY = trim(tempNewCompY);

				comp.emplace_back(serialToolMouse, tempNewCompX, tempNewCompY, 0);
				comp.back().stimuli();

				/*Collisions*/
				tempNewCompX = trim(tempNewCompX, gap);
				while (occupiedAt(comp.back(), sf::Vector2f(tempNewCompX, tempNewCompY))) {
					tempNewCompX = trim(tempNewCompX + 6 * gap);

					if (tempNewCompX + 7 * gap - 150 - view.getCenter().x + W / 2 + 91 >= W) {
						tempNewCompX = 150 + view.getCenter().x - W / 2;
						tempNewCompY = trim(tempNewCompY + 6 * gap);
					}
				}

				comp.back().x = (int)trim(tempNewCompX); comp.back().y = (int)trim(tempNewCompY);
				comp.back().stimuli();

				stimuliEndNodes = 1;
				CircuitGUI::qtUpdate();
			}
			else {
				mouseOnCompsBool = 0;

				if (virSerial.size() > 0 && allSqr.getGlobalBounds().contains(cursorInSim())) mouseOnCompsBool = 1;
				else {

					/*Check every component for Mouse*/
					for (int c = 0; /*!mouseOnCompsBool &&*/ c < comp.size(); c++) {

						if (comp[c].bounds.contains(cursorInSim())) {

							mouseOnCompsBool = 1; Drag = 0;

							if (!ShiftPressed) {
								CircuitGUI::virSerial.clear();
								CircuitGUI::virSprite.clear();
								CircuitGUI::virSerialShift.clear();
							}
							virSerial.emplace_back(c);

							//copied in sel square selection
							virSprite.emplace_back(comp[virSerial.back()].sprite);
							//virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
							virSprite.back().setOrigin(comp[virSerial.back()].sprite.getOrigin());
							virSprite.back().setColor(tempDimColor);/*
							for (int v = 0; v < virSerial.size(); v++) {//copied in sel square selection
								virSprite.emplace_back(comp[virSerial[v]].sprite);
								virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
								virSprite.back().setColor(tempDimColor);
							}*/

							break;
						}
					}
					updateAllSqr();

					/*SelSqr*/
					if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; selSqr.setPosition(cursorInSim()); }
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
			if (/*//asdf!PlayMode &&*/CircuitGUI::Click(CircuitGUI::gap)) {
				stimuliDisplay = 1; /*cout << "8";*/

				// Open/Close the Switch
				using namespace CircuitGUI;
				for (int v = 0; v < virSerial.size(); v++) {
					if (comp[virSerial[v]].bounds.contains(cursorInSim())) {

						// Changing Serial
						if (comp[virSerial[v]].getSerial() == Entity::SwC)
							comp[virSerial[v]].setSerial(Entity::SwO);
						else
							if (comp[virSerial[v]].getSerial() == Entity::SwO)
								comp[virSerial[v]].setSerial(Entity::SwC);

						virSerial.erase(virSerial.begin() + v);
						virSprite.erase(virSprite.begin() + v);

						break;
					}

				}

			}

			if (CircuitGUI::Click(3)) {
				CircuitGUI::virSprite.clear();  ///// 
				CircuitGUI::virSerial.clear();  ///// 
			}
		}
	}

	// Continoue while hold
	{
		if (!selectSquare && mouseOnCompsBool /*//asdf&& !PlayMode*/ && !CircuitGUI::Click(0) /*&& releaseBool*/) {
			using namespace CircuitGUI;

			static sf::Vector2f offsetPos;
			static sf::FloatRect virArea;

			virArea = allSqr.getGlobalBounds();
			offsetPos.x = (int)cursorInSim().x - (virArea.left + (int)(virArea.width / 2));
			offsetPos.y = (int)cursorInSim().y - (virArea.top + (int)(virArea.height / 2));

			for (int v = 0; v < virSprite.size(); v++)
				virSprite[v].setPosition(offsetPos.x + comp[virSerial[v]].x, offsetPos.y + comp[virSerial[v]].y);

			offsetPos = trim(offsetPos);

			bool moveAll = true;
			sf::Vector2f temp;
			for (int v = 0; v < virSerial.size(); v++) {
				temp.x = offsetPos.x + comp[virSerial[v]].x;
				temp.y = offsetPos.y + comp[virSerial[v]].y;

				if (occupiedAt(comp[virSerial[v]], temp, true)) {
					moveAll = 0;
					break;
				}
			}
			if (moveAll) {
				for (int v = 0; v < virSerial.size(); v++) {
					comp[virSerial[v]].x += (int)offsetPos.x; // += (were "=" before)
					comp[virSerial[v]].y += (int)offsetPos.y; // += (were "=" before)
					comp[virSerial[v]].stimuli();
				}
			}

			stimuliEndNodes = 1; stimuliDisplay = 1; /*cout << "9";*/
			qtUpdate();
		}
	}

	// Select Sqr
	{
		if (selectSquare && !releaseBool /*//asdf&& !PlayMode*/) {
			using namespace CircuitGUI;

			stimuliDisplay = 1; /*cout << "10";*/
			Selection = 1;
			/*Sel Sqr*/
			selSqr.setSize(cursorInSim() - selSqr.getPosition());


			/*Selection*/
			virSerial.clear();

			// 2nd Method  --QuadTrees
			qtExtract(selSqr.getGlobalBounds(), virSerial);

			// 1st Method  --Check every component
			/*for (int c = 0; c < CircuitGUI::comp.size(); c++) {
				//cout << std::count(virSerial.begin(), virSerial.end(), c);
				//bool bi = std::binary_search(virSerial.begin(), virSerial.end(), c);
				//auto mk = std::find(virSerial.begin(), virSerial.end(), c);
				int v = lower_bound(virSerial.begin(), virSerial.end(), c) - virSerial.begin();
				bool compFound = 0;
				if (v < virSerial.size()) compFound = (virSerial[v] == c);
				//bool compFound = 0;
				//int v = 0;
				//for (; v < CircuitGUI::virSerial.size() && c >= CircuitGUI::virSerial[v]; v++) { // if(CircuitGUI::virSerial[v]) break; // Ensures the virSerial is Sorted by breaking at the right time
				//	if (c == CircuitGUI::virSerial[v]) { compFound = 1; break; }
				//}
				//(!(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift)));


				if (CircuitGUI::comp[c].bounds.intersects(CircuitGUI::selSqr.getGlobalBounds())) {
					if (!compFound)
					{
						CircuitGUI::virSerial.insert(CircuitGUI::virSerial.begin() + v, c);
						//CircuitGUI::virSprite.insert(CircuitGUI::virSprite.begin() + v, CircuitGUI::comp[c].sprite);
						//CircuitGUI::virSprite[v].setColor(tempDimColor);

						//virSprite.back().setOrigin(virSprite.back().getTexture()->getSize().x / 2, virSprite.back().getTexture()->getSize().y / 2);
						//virSprite.back().setColor(tempDimColor);
					}
				}
				else if (compFound)
				{
					CircuitGUI::virSerial.erase(CircuitGUI::virSerial.begin() + v);
					//CircuitGUI::virSprite.erase(CircuitGUI::virSprite.begin() + v);
				}
			}*/

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
		else { CircuitGUI::selSqr.setSize(CircuitGUI::zero); }
	}

	// Wire
	{
		using namespace CircuitGUI;

		{
			static int index = 0;
			static bool onWire = false;

			if (releaseBool && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				onWire = false;
				for (int i = 0; i < wires.size(); i++) {
					if (!onWire && wires[i].contains(cursorInSim())) {
						onWire = true; index = i;
					}
				}
			}

			if (onWire && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				wires[index].move(cursorInSim());

		}

		if (wires.empty() == false)
			if (wires.back().isStopped() == false) {
				wires.back().makeWire();
				//stimuliDisplay = 1;/*LOG("\nmakeWire()");*/
			}
	}
}

void App::ImGUI()
{
	using namespace CircuitGUI;

	ImGui::SFML::Update(app, deltaClock.restart());
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

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			stimuliDisplay = 1; /*cout << "a";*/
			if (ImGui::MenuItem("Open...", "Ctrl + O")) {
				stimuliDisplay = 1; /*cout << "14";*/ stimuliEndNodes = 1;

				std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

				if (!filepath.empty())
					CircuitGUI::Options::Open(filepath);
			}
			if (ImGui::MenuItem("Save", "Ctrl + S")) {
				;
				//_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
				//_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)
			}
			if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {

				_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
				_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)

				std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

				if (!filepath.empty()) {

					if (filepath.back() == 'T')
						CircuitGUI::Options::Save(filepath);

					if (filepath.back() == 'G')
						CircuitGUI::Options::SaveAsImage(filepath);

				}

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Esc")) { CircuitGUI::app.close(); End = 1; /*continue;*/ }

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

					darkLightMode = false;

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

				CircuitGUI::comp.clear();
				CircuitGUI::virSerial.clear();
				CircuitGUI::virSprite.clear();
				CircuitGUI::virSerialShift.clear();

				//asdfwires.clear();

				if (PlayMode) {
					CircuitGUI::Options::openf("Saved-Projects\\Maze.TXT");
					//ShellExecute(0, 0, L"https://www.youtube.com/watch?v=6cRctjPRv6M", 0, 0, SW_SHOW);
				}
				else { wireBool = 0; PlayRot = 0; }
			}*/

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Analysis")) {
			stimuliDisplay = 1; /*cout << "a";*/

			if (ImGui::MenuItem("Generate Graph")) {
				stimuliDisplay = 1; /*cout << "114";*/

				//circuit.clearAll();

				//for (int i = 0; i < allEnds.size(); i++)
				//	circuit.newItem(i);
				//
				//for (auto& component : comp)
				//	circuit.link(component.node1, component.node2);
			}

			if (ImGui::MenuItem("Print Graph")) {
				//circuit.printGraph();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			stimuliDisplay = 1; //cout << "c";
			if (ImGui::BeginMenu("Controls"))
			{
				if (ImGui::BeginTable("table_context_menu_2", 2))
				{

					static const std::string opt[11][2] = { //HardCode
						{ "Drag View", "Press Scroll-Wheel + Drag" },
						{ "Selection Mode", "Click + Hold + Drag" },
						{ "Select All",		"Ctrl + A" },
						{ "Rotate",			"Ctrl + R" },
						{ "Delete",			"Del" },
						{ "Copy",			"Ctrl + C" },
						{ "Paste",			"Ctrl + V" },
						{ "Open",			"Ctrl + O" },
						{ "Save As",		"Ctrl + Shift + S" },
						{ "Escape",			"Esc" },
						{ "New Components",	"Hover Left" }
					};

					ImGui::TableSetupColumn("Option");
					ImGui::TableSetupColumn("Keys");

					ImGui::TableHeadersRow();
					ImGui::TableNextRow();

					for (int row = 0; row < 11; row++) //HardCode
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", opt[row][0].c_str());
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
					ImGui::Text("%s", links[i][1].c_str());
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


		ImGui::EndMainMenuBar();
	}

	/*
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			static ImGuiTabBarFlags tab_items_flags = ImGuiTabBarFlags_Reorderable;
			static bool a = 1, b = 1, c = 1;
			std::string fileNameIm = "Untitled-" + std::to_string(10) + ".txt";
			if (ImGui::BeginTabItem(fileNameIm.c_str(), &a, tab_items_flags))
			{
				//ImGui::Text("This is the Avocado tab!");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Broccoli", &b, tab_items_flags))
			{
				//ImGui::Text("This is the Broccoli tab!");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Cucumber", &c, tab_items_flags))
			{
				//ImGui::Text("This is the Cucumber tab!");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}//*/

	/*
			ImGui::SFML::Update(app, deltaClock.restart());
			// Enable docking
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			// Create docking space
			ImGui::DockSpaceOverViewport();

			{
				// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
				// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
				// In this specific demo, we are not using DockSpaceOverViewport() because:
				// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
				// - we allow the host window to have padding (when opt_padding == true)
				// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
				// TL;DR; this demo is more complicated than what you would normally use.
				// If we removed all the options we are showcasing, this demo would become:
				//     void ShowExampleAppDockSpace()
				//     {
				//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
				//     }

				static bool opt_fullscreen = true;
				static bool opt_padding = false;
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				if (opt_fullscreen)
				{
					const ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->WorkPos);
					ImGui::SetNextWindowSize(viewport->WorkSize);
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				}
				else
				{
					dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
				}

				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
				// and handle the pass-thru hole, so we ask Begin() to not render a background.
				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
				// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
				// all active windows docked into it will lose their parent and become undocked.
				// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
				// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
				if (!opt_padding)
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				// Begin main dockspace
				//ImGui::Begin("DockSpace Demo", nullptr, window_flags);
				ImGui::Begin("Main Dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
				if (!opt_padding)
					ImGui::PopStyleVar();

				if (opt_fullscreen)
					ImGui::PopStyleVar(2);

				// Submit the DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Options"))
					{
						// Disabling fullscreen would allow the window to be moved to the front of other windows,
						// which we can't undo at the moment without finer window depth/z control.
						ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
						ImGui::MenuItem("Padding", NULL, &opt_padding);
						ImGui::Separator();

						if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
						if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
						if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
						if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
						if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
						ImGui::Separator();
						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}


				ImGui::Begin("Settings");
				ImGui::Button("Click Me");
				float value = 10.0f, value2[2] = { 15.0f,16.0f };
				ImGui::DragFloat("Value", &value);
				ImGui::DragFloat2("Values", value2);
				ImGui::End();



				ImGui::End();
			}
			*/

	/*if (ImGui::TreeNode("Advanced & Close Button"))
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
				}*/

	/*ImGui::Dummy(ImVec2(0, 20));

					ImGui::BeginTabBar("#Additional Parameters");
					float value = 0.0f;
					if (ImGui::BeginTabItem("Tab Name2")) {
						ImGui::SliderFloat("Slider", &value, 0, 1.0f);
					}
					if (ImGui::BeginTabItem("Tab Name3")) {
						ImGui::Text("Tab 2");
					}
					if (ImGui::BeginTabItem("Tab Name4")) {
						ImGui::Text("Tab 3");
					}
					if (ImGui::BeginTabItem("Tab Name5")) {
						ImGui::Text("Tab 4");
					}
					ImGui::EndTabBar();*/

	/*ImGui::Begin("Right-Click");
						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::Selectable("Apple")) LOG("\nApple");
							if (ImGui::Selectable("Banana")) LOG("\nBanana");
							ImGui::EndPopup();
						}
						ImGui::End;*/

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

}

void App::Update()
{
	using namespace CircuitGUI;

	// Drag
	{
		if (Drag) {
			stimuliDisplay = 1; /*cout << "18"*/;
			CircuitGUI::Drag();
			CircuitGUI::colorBrightLineGrid();
			CircuitGUI::updatePosToolBox();
			CircuitGUI::updateVisibleVector();
		}
		//testCircle.setPosition(CircuitGUI::onScreen(300, 300));
	}

	//Tool Win
	{
		//if (MInTool) { stimuliDisplay = 1; /*cout << "\nMInTool == true;\tIn Update{}";*/ }
		if (Drag)  toolCol.setPosition(onScreen(0, 0));
		else toolCol.setPosition((MInTool) * (t_TollWx + (toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MInTool) * (t_TollWx + (toolWinRestPos.x - c_toolColWidth - t_TollWx) / 7), toolWinRestPos.y);
	}

	// Tool Sqr
	{
		t_TollWx = ToolBoxLittleBox.getPosition().x;
		if (!Drag) ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MIntool) * (t_TollWx + (toolWinRestPos.x - c_toolColWidth - t_TollWx) / 7), toolWinRestPos.y + trim(sf::Mouse::getPosition(app).y, c_toolColWidth));
		if (Drag)  ToolBoxLittleBox.setPosition(onScreen(0, 0 + trim(sf::Mouse::getPosition(app).y, c_toolColWidth)));
		if (MIntool) serialToolMouse = (int)(sf::Mouse::getPosition(app).y / c_toolColWidth); else serialToolMouse = 0;
	}

	// endNodes
	{
		if (stimuliEndNodes)
		{
			using namespace CircuitGUI;

			updateAllEnds();

			updateEndCircles();

			updateAllSqr();

			qtUpdate();

			/*
			if (comp.size() < 1000)
				qtWrite();
			else LOG("\nNot Printing QuadTree [No. of Elements(" << comp.size() << ") is more that 1000]\n\t* Not Printing because the sheer amount of time it would take\n");//*/

			updateVisibleVector();

			{
				//circuit.clearAll();
				//
				//for (int c = 0; c < allEnds.size(); c++)
				//	circuit.newItem(c);
				//
				//for (auto& component : comp)
				//	circuit.link(component.node1, component.node2);
				//
				//circuit.printGraph();
			}
		}
	}

	//circuit.updateWin();

	/*asdfif (PlayRot) { CircuitGUI::view.rotate(0.9); }
	else CircuitGUI::view.setRotation(0);*/
}

void App::Render()
{
	if (1 || stimuliDisplay)
	{
		using namespace CircuitGUI;

		app.setView(view);

		app.clear(backColor);

		drawGrid();

		drawAllSqr();

		qtDraw(qt);

		drawComp();

		drawWires();

		drawNodes();

		drawBoarders();

		drawVirSprites();

		//for (auto& e : newComps) e->draw(app);

		drawSelSqr();

		drawToolColumn(MInTool, MIntool);

		//if (DrawCircle) app.draw(testCircle);

		ImGui::SFML::Render(app); // Last Thing to render
	}

	CircuitGUI::app.display();
}

void App::EndFrame()
{
	CircuitGUI::app.setTitle("CircuitSim   " + std::to_string((float)(((float)clock() - (float)FrameTime_for_FPS) / (float)CLOCKS_PER_SEC) * 1000.0F) + " | " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)FrameTime_for_FPS))));
	
	FrameTime_for_FPS = clock();
	
	stimuliDisplay = 0; stimuliEndNodes = 0; CircuitGUI::Occupied = 0;
}

