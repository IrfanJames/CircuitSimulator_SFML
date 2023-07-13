/*
	* Colapse all Scopes - it'll be easier
	* Ctrl + M + A in Visual Studio
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <direct.h>
#include <future>
//#include <thread>
//#include <sstream>
//#include <string>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "LOG.hpp"
#include "Circuit_Entity.hpp"
#include "Circuit_Windows_Stuff.hpp"
#include "Circuit_Wire.hpp"
#include "Circuit_Item.hpp"
#include "Circuit_Graph.hpp"
#include "Resource_Manager.hpp"

#include "Circuit_GUI.hpp"
#include "CircuitCore.hpp"
#include "Circuit_App.hpp"



App::App(const std::vector<std::string>& filepaths)
{
	srand(time(NULL));

	CircuitGUI::initializeGUI();

	ImGui::SFML::Init(CircuitGUI::app);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	time_t FrameTime_for_FPS = clock();


	//Resource my_pic(IDR_CAP, "BMP");
	//Resource my_font(IDR_FONT2, "FONT");
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
#else
	LOG::log_file.close();
#endif
}

void App::Run()
{

	//////////////// Main Loop ////////////////////
	while (CircuitGUI::app.isOpen() && End == false)
	{
		Events();

		ImGUI();

		Options();

		Threads();

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

			view = sf::View(sf::FloatRect(
				(int)view.getCenter().x - (int)(evnt.size.width / 2),
				(int)view.getCenter().y - (int)(evnt.size.height / 2),
				(int)evnt.size.width,
				(int)evnt.size.height));
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

			CircuitGUI::updateVisibleVectors();
		}

		if (evnt.type == evnt.MouseButtonPressed)
		{
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

			if (evnt.mouseButton.button == sf::Mouse::Right) {
				RightClickMenu = true;
			}
			else RightClickMenu = false;

			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				dragBool = true;
				mouseOnCompsBool = 0;
				CircuitGUI::iniDrag();
			}
		}

		if (evnt.type == evnt.MouseButtonReleased)
		{
			if (evnt.mouseButton.button == sf::Mouse::Left) {

				releaseBool = 1;

				if (Selection) {
					Selection = 0;

					//virSprite.clear();
					virSprite.reserve(virSerial.size());
					/*for (int v = 0; v < virSerial.size(); v++) {
						virSprite.emplace_back(comp[virSerial[v]].sprite);
						virSprite.back().setColor(CircuitGUI::tempDimColor); }*/


					for (int v = 0; v < virSerial.size() && v < virSprite.size(); v++)
					{
						virSprite[v] = comp[virSerial[v]].sprite;
						virSprite[v].setColor(CircuitGUI::tempDimColor);
					}

					if (virSerial.size() < virSprite.size())
					{
						virSprite.erase(virSprite.begin() + (int)virSerial.size(), virSprite.end());
					}

					if (virSerial.size() > virSprite.size())
					{
						for (int v = virSprite.size(); v < virSerial.size(); v++)
						{
							virSprite.emplace_back(comp[virSerial[v]].sprite);
							virSprite.back().setColor(CircuitGUI::tempDimColor);
						}
					}

				}
			}

			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				dragBool = false;
			}
		}

		if (evnt.type == evnt.KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::Escape) {
				app.close();
				End = 1;
				continue;
			}
			if (evnt.key.code == sf::Keyboard::Delete) {
				if (ImGuiInFocus) return;

				stimuliDisplay = 1; /*cout << "2";*/	stimuliEndNodes = 1;
				CircuitGUI::Options::Delete();
			}
			if (evnt.key.code == sf::Keyboard::Tab)
			{
				static int serialComp_Tab = 0;
				
				if (visibleComps.empty() == false)
				{
					if (serialComp_Tab == visibleComps.size())
					{
						serialComp_Tab = 0;
						virSerial.clear();
					}
					else if (serialComp_Tab <= visibleComps.size() - 1)
					{
						virSerial.clear();
						virSprite.clear();

						virSerial.emplace_back(visibleComps[serialComp_Tab]);
						virSprite.emplace_back(comp[virSerial.back()].sprite);
						virSprite.back().setColor(tempDimColor);

						serialComp_Tab++;
					}
					else // Error: Index Out of Bounds
						serialComp_Tab = 0;

					CircuitGUI::updateVisibleVectors();
				}
			}
			if (evnt.key.code == sf::Keyboard::L) {

				LOG("\n\nwires (" << wires.size() << "): ");
				for (auto& w : wires)
					LOG(w.noOfEdges() << " ");

				LOG("\ncursor: "); LOG_VEC2(cursorInSim());

				LOG("\n");
			}
			if (evnt.key.code == sf::Keyboard::T) {

				//static int i = -1;
				//if (comp.empty() == false && ++i <= comp.size() - !comp.empty())
				//{
				//	LOG("\n" << i << ": ");
				//	qtRemove(i);
				//	qtWrite();
				//}
				//else i = -1;

				//futures.emplace_back(std::async(std::launch::async, CircuitGUI::Options::SaveAsImage, "Saved-Images/asdf.PNG"));
				//futures.emplace_back(std::async(std::launch::async, CircuitGUI::EndLessPit));
				//threads.emplace_back(EndLessPit);

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
			/*if (evnt.key.code == sf::Keyboard::Y) {
				qtClean();
				qtWrite();
			}
			if (evnt.key.code == sf::Keyboard::W) qtWrite();*/


			// Up, Down, Right, Left Arrows
			{
				if (ImGuiInFocus) return;

				// Moving Components
				if (virSerial.empty() == false) {

					int dx = 0;
					int dy = 0;

					if (evnt.key.code == sf::Keyboard::Up) { dy = -gap; }
					if (evnt.key.code == sf::Keyboard::Down) { dy = +gap; }
					if (evnt.key.code == sf::Keyboard::Right) { dx = +gap; }
					if (evnt.key.code == sf::Keyboard::Left) { dx = -gap; }
					
					if (dx != 0 || dy != 0) {

						for (int v : virSerial) {

							comp[v].x += dx;
							comp[v].y += dy;

							comp[v].stimuli();

							stimuliDisplay = 1;
							stimuliEndNodes = 1;
						}
					}

				}

				// Dragging the View
				else {

					;

					/*int dx = 0;
					int dy = 0;

					if (evnt.key.code == sf::Keyboard::Up) { dy = -gap; }
					if (evnt.key.code == sf::Keyboard::Down) { dy = +gap; }
					if (evnt.key.code == sf::Keyboard::Right) { dx = +gap; }
					if (evnt.key.code == sf::Keyboard::Left) { dx = -gap; }

					view.move(dx, dy);*/
				}
			}

			// Ctrl
			if (evnt.key.control) {
				if (evnt.key.code == sf::Keyboard::A) {

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

					stimuliDisplay = 1;
					CircuitGUI::updateAllSqr();
					CircuitGUI::updateVisibleVectors();
				}
				if (evnt.key.code == sf::Keyboard::R) {

					CircuitGUI::Options::Rotate();

					stimuliDisplay = 1;	stimuliEndNodes = 1;
				}
				if (evnt.key.code == sf::Keyboard::C) {

					CircuitGUI::Options::Copy();
				}
				if (evnt.key.code == sf::Keyboard::X) {

					CircuitGUI::Options::Copy();
					CircuitGUI::Options::Delete();

					stimuliDisplay = 1;	stimuliEndNodes = 1;
				}
				if (evnt.key.code == sf::Keyboard::V) {

					CircuitGUI::Options::Paste();

					stimuliDisplay = 1; stimuliEndNodes = 1;
				}
				if (evnt.key.code == sf::Keyboard::O) {

					std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

					if (!filepath.empty())
						CircuitGUI::Options::Open(filepath);

					stimuliDisplay = 1; stimuliEndNodes = 1;
					CircuitGUI::updateVisibleVectors();
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
							futures.emplace_back(std::async(std::launch::async, CircuitGUI::Options::SaveAsImage, filepath));

					}

				}
			}

			//if (evnt.key.code == sf::Keyboard::W) { wireBool = !wireBool; }
			//if (evnt.key.code == sf::Keyboard::N) { debugBool = !debugBool;}
			/*if (evnt.key.code == sf::Keyboard::P) {
				LOG("\nCurrent Frame");
				sf::Texture texture;
				texture.create(app.getSize().x, app.getSize().y);
				texture.update(app, -40, -50);

				static int count = 0;
				sf::Image screenshot = texture.copyToImage();
				screenshot.saveToFile("screenshot-" + std::to_string(count++) + ".png");
			}*/

		}

		; {

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

void App::ImGUI() {
	using namespace CircuitGUI;
	ImGuiInFocus = false;

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

	if (ImGui::BeginMainMenuBar())
	{
		if (/*ImGui::IsWindowFocused() || */ ImGui::IsWindowHovered()) {
			//LOG("\nMainBar");
			ImGuiInFocus = true;
		}
		if (ImGui::BeginMenu("File"))
		{
			ImGuiInFocus = true;
			stimuliDisplay = 1; /*cout << "a";*/

			if (ImGui::MenuItem("New", "Ctrl + N")) {
				// stimuliDisplay = 1;
			}
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
						//futures.emplace_back(std::async(std::launch::async, CircuitGUI::Options::SaveAsImage, filepath));
				}

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Esc")) { CircuitGUI::app.close(); End = 1; /*continue;*/ }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGuiInFocus = true;
			stimuliDisplay = 1; /*cout << "b";*/

			if (ImGui::BeginMenu("Handdrawn Icons"))
			{
				ImGui::Text("Coming Soon...");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			//ImGui::MenuItem("Show QuadTree", "", &visible_QuadTree);
			if (ImGui::BeginMenu("Theme")) {

				if (ImGui::MenuItem("Dark")) {
					stimuliDisplay = 1; /*cout << "15";*/

					darkLightMode = false;

					updateThemeColors();
				}
				if (ImGui::MenuItem("Light")) {
					stimuliDisplay = 1; /*cout << "16";*/

					darkLightMode = true;

					updateThemeColors();
				}

				ImGui::EndMenu();
			}
			ImGui::Checkbox("Show QuadTree", &visible_QuadTree);


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
		if (ImGui::BeginMenu("Simulation"))
		{
			ImGuiInFocus = true;
			stimuliDisplay = 1; /*cout << "a";*/

			if (ImGui::MenuItem("Generate Graph"))
			{
				stimuliDisplay = 1; /*cout << "114";*/

				circuit.clearAll();

				//LOG("\n");
				for (int i = 0; i < allEnds.size(); i++)
				{
					//LOG("\n");
					//LOG_VEC2(allEnds[i]);
					circuit.newItem_noSetGUI(i);
				}

				for (auto& component : comp)
					circuit.link_noSetGUI(component.node1, component.node2);

				circuit.setGraph();

				circuit.createWindow();
				//circuit.updateWin();
			}
			if (ImGui::MenuItem("Print Graph"))
			{
				circuit.printGraph();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGuiInFocus = true;
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

		/*ImGui::Begin("Right-Click");
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::Selectable("Apple")) LOG("\nApple");
			if (ImGui::Selectable("Banana")) LOG("\nBanana");
			ImGui::EndPopup();
		}
		ImGui::End;

		if (ImGui::BeginPopup("Set Value")) {

			static float value = 0;
			ImGui::DragFloat("Value", &value);

			comp[virSerial.front()].voltage = value;

		}
		ImGui::EndPopup();*/

		if (ImGui::Checkbox("", &visible_QuadTree)) {
			ImGuiInFocus = true;
		}

		//if (ImGui::SmallButton("AD")) {
		//	LOG("\nAds");
		//}
		//
		//if (ImGui::Button("ad")) {
		//	LOG("\nAds");
		//}

		ImGui::EndMainMenuBar();
	}

	//Docking
	; {
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::Begin("Docking Space", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

		ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		//ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();
	}

	if (ImGui::Begin("Window-A##ID"))
	{
		if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
		{
			ImGuiInFocus = true;
			//LOG("O");
		}

		ImGui::Text("Hello");

		if (ImGui::Button("Apple"))
			LOG("\nApple");

		if (ImGui::Button("Banana"))
			LOG("\nBanana");

		static int prefix = 4;
		static float sliderValue = -0.123;

		// Slider
		{
			//if (virSerial.empty() == false)
			//{
			//	double value = comp[virSerial.front()].value;
			//	bool isNegative = value < 0;
			//
			//	sliderValue = value;
			//	prefix = 4;
			//
			//	value = std::abs(value);
			//
			//	if (value == 0) { sliderValue = 0; prefix = 4; }
			//	else if (value < 1) { ; }
			//	else { ; }
			//
			//}


			float sensitivity = (abs(sliderValue) <= 1.1f) ? 0.01f : std::log10f(std::abs(sliderValue));
			//ImGui::SetNextItemWidth(120);
			ImGui::DragFloat("Value", &sliderValue, sensitivity, -999.99999f, 999.99999f);

			//ImGui::Text("New Value:");
			//ImGui::SameLine();
			//
			//static char valueInput[32];
			//ImGui::InputText("##ValueInput", valueInput, sizeof(valueInput)); // Crashes on Ctrl+V
			//
			//bool safe = false;
			//double newValue = 0;
			//std::string test(valueInput);
			//if (test.empty() == false &&												  // Input String not Empty
			//	('0' <= test[0] && test[0] <= '9') ||									  // 1st character is a digit [0-9]
			//	(test[0] == '-' && 2 <= test.size() && '0' <= test[1] && test[1] <= '9')) // In case of Negative Numbers: 1st char is '-' and 2nd char is a digit [0-9]
			//	safe = true;
			//
			//if (safe) newValue = std::stod(test);
			//
			// LOG("\ntest: " << test);
			// LOG("\tnewValue: " << newValue);
		}

		// Prefix
		{
			static const std::string prefixes[8] = { "p", "n", "u", "m", "1", "k", "M", "G" };

			for (int i = 0; i < 8; i++) {
				if (ImGui::RadioButton(prefixes[i].c_str(), prefix == i))
					prefix = i;

				if (i != 3) ImGui::SameLine();
			}
		}

		if (ImGui::Button("Apply")) {

			for (int i : virSerial) {
				comp[i].value = sliderValue * std::powl(10, (prefix - 4) * 3);
				comp[i].updateValueText();
			}
		}

	}
	ImGui::End();

	if (0 && RightClickMenu) {
		ImGui::SetNextWindowPos(ImGui::GetMousePos());
		if (ImGui::Begin("Window-B##ID"))
		{
			ImGui::Text("Hello");

			if (ImGui::Button("Apple"))
				LOG("\nApple");

			if (ImGui::Button("Banana"))
				LOG("\nBanana");

			ImGui::Text("New Value:");
			ImGui::SameLine();

		}
		ImGui::End();
		RightClickMenu = false;
	}


	//ImGui::SetNextWindowPos(ImVec2(70, 70));
	//ImGui::SetNextWindowPos(ImGui::GetMousePos());
	//if (ImGui::BeginPopup("Change Value")) {
	//	ImGui::Text("New Value:");
	//
	//	// Input field for the new value
	//	static char valueInput[32];
	//	ImGui::InputText("##ValueInput", valueInput, sizeof(valueInput));
	//
	//	if (ImGui::Button("Apply")) {
	//		// Parse the new value from the input field
	//		double newValue = std::stod(valueInput);
	//
	//		// Update the component value
	//		LOG("\nnewValue: " << newValue);
	//
	//		ImGui::CloseCurrentPopup();
	//	}
	//
	//	ImGui::EndPopup();
	//}

	//LOG("\nImGuiInFocus: " << ImGuiInFocus);
}

void App::Options() {

	if (ImGuiInFocus) return;

	//sf::Mouse Hold
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { //////////// Urgent need of enums , State Machine
		if (releaseBool)
		{
			using namespace CircuitGUI;
			releaseBool = false;
			click = clock();
			mouseHold = (sf::Vector2f)sf::Mouse::getPosition(app);

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

				if (virSerial.size() > 0 && allSqr.getGlobalBounds().contains(cursorInSim()))
				{	
					mouseOnCompsBool = 1;

					if (allSqr.getSize().x >= gap && allSqr.getSize().y >= gap)
						mouseOffSet = allSqr.getPosition() - cursorInSim();
				}
				else {

					/*Check every component for Mouse*/
					static sf::FloatRect searchArea;
					static std::vector<int> vec;

					{
						int halfSide = 5;
						sf::Vector2f cursor = cursorInSim();

						searchArea.left = cursor.x - halfSide;
						searchArea.top = cursor.y - halfSide;
						searchArea.width = 2 * halfSide;
						searchArea.height = 2 * halfSide;
					}

					qtExtract(searchArea, vec);

					//for (int c = 0; /*!mouseOnCompsBool &&*/ c < comp.size(); c++) {
					for (int i : vec) {

						if (comp[i].bounds.contains(cursorInSim())) {

							mouseOnCompsBool = 1; dragBool = false;

							if (!ShiftPressed) {
								virSerial.clear();
								virSprite.clear();
								virSerialShift.clear();
							}
							virSerial.emplace_back(i);

							//LOG("\n\ncomp[c].boundswidth: " << comp[c].bounds.width);
							//LOG("\ncomp[c].boundsheight: " << comp[c].bounds.height);
							//LOG("\ncomp[c].boundsleft: " << comp[c].bounds.left);
							//LOG("\ncomp[c].boundstop: " << comp[c].bounds.top);
							CircuitGUI::updateAllSqr();
							mouseOffSet = allSqr.getPosition() - cursorInSim();

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

					/*SelSqr*/
					if (!mouseOnCompsBool /*Wheel*/ /*&& selectSquare*/) { selectSquare = 1; selSqr.setPosition(cursorInSim()); }
				}

				/*Wheel*/
				/*Drag Background*/
				/*if (!mouseOnCompsBool && !selectSquare) {
					dragBool = 1; mouseOnCompsBool = 0;
					viewX = view.getCenter().x, viewY = view.getCenter().y;
					verX = vLines[0].getPosition().x; verY = vLines[0].getPosition().y;
					horX = hLines[0].getPosition().x; horY = hLines[0].getPosition().y;
				}*/
			}


			// Move Wire
			{
				onWireIndex = -1;
				for (int i = 0; i < wires.size(); i++) {
					if (onWireIndex < 0 && wires[i].contains(cursorInSim())) {
						onWireIndex = i;
					}
				}
			}

		}
	}
	else {
		/*Wheel*//*dragBool = 0;*/ mouseOnCompsBool = 0; selectSquare = 0;

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
		using namespace CircuitGUI;

		// Components
		{
			if (!selectSquare && mouseOnCompsBool /*//asdf&& !PlayMode*/ && !Click(0) /*&& releaseBool*/ && !makingWire()) {

				static sf::Vector2f offsetPos;
				static sf::FloatRect virArea;

				virArea = allSqr.getGlobalBounds();

				offsetPos.x = (int)cursorInSim().x - (virArea.left/* + (int)(virArea.width/ 2)*/) + (int)mouseOffSet.x;
				offsetPos.y = (int)cursorInSim().y - (virArea.top /*+ (int)(virArea.height/ 2)*/) + (int)mouseOffSet.y;


				for (int v = 0; v < virSprite.size(); v++)
					virSprite[v].setPosition(offsetPos.x + comp[virSerial[v]].x, offsetPos.y + comp[virSerial[v]].y);

				offsetPos = trim(offsetPos);

				bool moveAll = true;
				sf::Vector2f temp;
				for (int v : virSerial) {
					temp.x = offsetPos.x + comp[v].x;
					temp.y = offsetPos.y + comp[v].y;

					if (occupiedAt(comp[v], temp, true)) {
						moveAll = 0;
						break;
					}
				}
				if (moveAll) {
					for (int v : virSerial) {
						//qtRemove(v);

						comp[v].x += (int)offsetPos.x; // += (were "=" before)
						comp[v].y += (int)offsetPos.y; // += (were "=" before)
						comp[v].stimuli();

						//qtAdd(v, qt);
					}

					stimuliEndNodes = 1;
				}

				stimuliDisplay = 1;
				//stimuliEndNodes = 1;
				// CircuitGUI::qtUpdate();
			}
		}

		// Wires
		{
			if (0 <= onWireIndex && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				wires[onWireIndex].move(cursorInSim());
		}
	}


	// Select Sqr
	{
		if (selectSquare && !releaseBool /*//asdf&& !PlayMode*/) {
			using namespace CircuitGUI;

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

			stimuliDisplay = 1; Selection = 1;
			CircuitGUI::updateAllSqr();
			CircuitGUI::updateVisibleVectors();
		}
		else { CircuitGUI::selSqr.setSize(CircuitGUI::zero); }
	}


	// Making Wire
	{
		using CircuitGUI::wires;

		if (wires.empty() == false)
			if (wires.back().isStopped() == false) {
				wires.back().makeWire();
				// stimuliDisplay = 1;
			}
	}

}

void App::Threads()
{
	for (int i = 0; i < futures.size(); i++) {

		std::future_status status = futures[i].wait_for(std::chrono::milliseconds(10));
		//LOG("\nThread(" << i << "): ");

		if (status == std::future_status::ready) //{
			// The future is ready (thread completed or value available)
		//	LOG("ready");

			futures.erase(futures.begin() + i--);
		//}
		//else if (status == std::future_status::timeout) {
		//	// The future is not ready within the specified duration
		//	LOG("timeout");
		//}
		//else if (status == std::future_status::deferred) {
		//	// The future is deferred (using std::promise and std::async(launch::deferred))
		//	LOG("deferred");
		//}

	}
}

void App::Update()
{
	using namespace CircuitGUI;

	// Drag
	{
		if (dragBool) {
			stimuliDisplay = 1; /*cout << "18"*/;
			CircuitGUI::Drag();
			CircuitGUI::colorBrightLineGrid();
			CircuitGUI::updatePosToolBox();
			CircuitGUI::updateVisibleVectors();
		}
		//testCircle.setPosition(CircuitGUI::onScreen(300, 300));
	}

	//Tool Win
	{
		//if (MInTool) { stimuliDisplay = 1; /*cout << "\nMInTool == true;\tIn Update{}";*/ }
		if (dragBool)  toolCol.setPosition(onScreen(0, 0));
		else toolCol.setPosition((MInTool) * (t_TollWx + (toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MInTool) * (t_TollWx + (toolWinRestPos.x - c_toolColWidth - t_TollWx) / 7), toolWinRestPos.y);
	}

	// Tool Sqr
	{
		t_TollWx = ToolBoxLittleBox.getPosition().x;
		if (!dragBool) ToolBoxLittleBox.setPosition((MIntool) * (t_TollWx + (toolWinRestPos.x + 0 - t_TollWx) / 7) + (!MIntool) * (t_TollWx + (toolWinRestPos.x - c_toolColWidth - t_TollWx) / 7), toolWinRestPos.y + trim(sf::Mouse::getPosition(app).y, c_toolColWidth));
		if (dragBool)  ToolBoxLittleBox.setPosition(onScreen(0, 0 + trim(sf::Mouse::getPosition(app).y, c_toolColWidth)));
		if (MIntool) serialToolMouse = (int)(sf::Mouse::getPosition(app).y / c_toolColWidth); else serialToolMouse = 0;
	}

	// endNodes
	{
		if (stimuliEndNodes)
		{
			CircuitGUI::updateAllSqr();

			CircuitGUI::qtUpdate();

			CircuitGUI::updateAllEnds();
			
			CircuitGUI::updateEndCircles();

			CircuitGUI::updateVisibleVectors();

			//{
			//	circuit.clearAll();
			//	
			//	for (int c = 0; c < allEnds.size(); c++)
			//		circuit.newItem(c);
			//	
			//	for (auto& component : comp)
			//		circuit.link(component.node1, component.node2);
			//	
			//	circuit.printGraph();
			//}
		}
	}

	circuit.updateWin();

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

		//drawAllSqr();

		qtDraw(qt);

		drawComp();

		drawWires();

		drawNodes();

		drawBoarders();
		drawAllSqr();

		drawVirSprites();

		//for (auto& e : newComps) e->draw(app);

		drawSelSqr();

		drawToolColumn(MInTool, MIntool);

		//if (DrawCircle) app.draw(testCircle);

		ImGui::SFML::Render(app); // Last Thing to render
	}

	// CircuitGUI::app.display(); // Inside void App::EndFrame()
}

void App::EndFrame()
{
	CircuitGUI::app.display();

	CircuitGUI::app.setTitle("CircuitSim   " + std::to_string((float)(((float)clock() - (float)FrameTime_for_FPS) / (float)CLOCKS_PER_SEC) * 1000.0F) + " | " + std::to_string((float)((float)CLOCKS_PER_SEC / ((float)clock() - (float)FrameTime_for_FPS))));

	FrameTime_for_FPS = clock();

	stimuliDisplay = 0; stimuliEndNodes = 0; CircuitGUI::Occupied = 0;

}

