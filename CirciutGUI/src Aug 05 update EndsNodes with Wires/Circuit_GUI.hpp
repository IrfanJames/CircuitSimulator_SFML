
// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
#pragma once

//#include <iostream>
//#include <windows.h> //To Open file(txt/PNG) just after creating(Save as...) it
#include <fstream>

#include "SFML/Graphics.hpp"

#include "Circuit_Global.hpp"
#include "Circuit_Entity.hpp"
#include "Circuit_Wire.hpp"
#include "Circuit_Item.hpp"
#include "Resource_Manager.hpp"

//extern sf::Texture compTex[8];
extern std::vector<sf::Texture> compTex;


namespace CircuitGUI {

	/*Constants*/
	extern const int gap;
	extern const sf::Vector2f zero;
	extern const sf::Color normalCompColor;

	extern bool Occupied;
	extern bool darkLightMode;
	extern sf::Color tempDimColor;

	// Textures	
	//void loadTextures()
	//extern Resource Resource_Logo;
	//extern Resource Resource_Font;
	//extern std::vector<Resource> Resource_Images;

	extern sf::Event evnt;
	extern sf::RenderWindow app;
	//renderWinInit();
	extern sf::View view;
	//viewInit();


	// Cursor
	//extern float mouseHoldX, mouseHoldY;
	extern sf::Vector2f mouseHold;
	extern sf::Vector2f mouseOffSet;
	extern time_t click; // Time passed since Click
	 
	extern bool Click(int Sensitivity);
	extern sf::Vector2f cursorInSim();
	extern sf::Vector2f onScreen(float X, float Y);

	
	// Warning: Clears the vec
	extern void str_to_vecInt(const std::string& str, std::vector<int>& vec);
	extern float trim(float num, int wrt = gap);
	extern sf::Vector2f trim(sf::Vector2f vec, int wrt = gap);


	// Grid
	//extern int virtualBoarder;
	//extern std::vector<sf::RectangleShape> vLines;
	//extern std::vector<sf::RectangleShape> hLines;
	//extern sf::Color gridColor;
	extern sf::Color backColor;
	//extern void colorGrid();
	//extern void colorBackground();
	extern void drawGrid();
	//void initializeGrid()


	// Drag
	extern bool dragBool;
	//extern float viewX, viewY;
	//extern float verX, verY;
	//extern float horX, horY;
	//extern int verBrightCount, horBrightCount;
	extern void iniDrag();
	extern void Drag();
	extern void colorBrightLineGrid();


	// Vectors
	extern std::vector<Entity> comp;
	extern std::vector<int> virSerial;
	extern std::vector<sf::Sprite> virSprite;
	extern std::vector<int> virSerialShift;
	extern std::vector<Wire> wires;
	extern std::vector<sf::Vector2f> allEnds;
	extern std::vector<sf::CircleShape> allEndCircles;
	extern std::vector<int> visibleComps;
	extern std::vector<int> visibleEndNodes;
	extern std::vector<int> visibleBoarders;

	//extern std::vector<std::vector<Entity>::iterator> newComps;
	//extern std::vector<Item> newItems;

	extern void drawComp();
	extern void drawVirSprites();
	extern void drawNodes();
	extern void drawBoarders();
	extern void drawWires();
	extern void updateAllEnds();
	extern void updateAllEndsbyWires();
	extern bool makingWire();
	
	//extern void EndLessPit();

	struct quadTree;
	extern quadTree qt;
	extern bool visible_QuadTree;
	//extern void qtClean(quadTree& box = qt);
	//extern void qtRemove(int c, quadTree& box = qt);
	extern void qtDelete(quadTree& box);
	extern void qtAdd(int c, quadTree& box);
	extern void qtUpdate();
	//extern void qtWrite(const quadTree& box = qt, int indentation = 0);
	extern void qtDraw(quadTree& box);
	extern void qtExtract(const sf::FloatRect& searchArea, std::vector<int>& output, const quadTree& box = qt, bool clearVector = true);


	extern bool occupiedAt(const Entity& en, const sf::Vector2f& At, bool ignoreAllVir = false);
	extern sf::FloatRect areaofCollection(int collection, bool* sucess_ptr = nullptr);
	extern void updateVisibleVectors();
	extern void addToVector(int integer, std::vector<int>& vec);


	extern void colorEntityboarder();
	extern sf::CircleShape nodePic;
	//void nodePicDesign()
	extern sf::RectangleShape selSqr;
	extern void selSqrDesign();
	extern void drawSelSqr();
	extern sf::RectangleShape allSqr;
	extern void allSqrDesign();
	extern void updateAllSqr();
	extern void drawAllSqr();


	/*ToolBox*/
	extern const float c_toolColWidth;
	extern sf::Vector2f toolWinRestPos;
	extern sf::RectangleShape toolCol;
	extern sf::RectangleShape ToolBoxLittleBox;
	//void toolBoxInit()
	//extern sf::Sprite ToolSpr[Entity::no_of_Comp - 1];
	//extern sf::Vector2f ToolSprPOS[Entity::no_of_Comp - 1];
	extern void updatePosToolBox();
	extern void drawToolColumn(bool MInTool, bool MIntool);


	extern void updateThemeColors();
	extern void initializeGUI();


	namespace Options
	{
		extern void Open(const std::string& filepath);

		extern void Save(const std::string& file);

		extern void SaveAsImage(const std::string& filepath);

		extern void Copy();

		extern void Paste();

		extern void Rotate();

		extern void Delete();
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
