#pragma once

#include <string>
#include <vector>

#include "Circuit_Graph.hpp"

class App {

public:

	App(const std::vector<std::string>& filepaths);

	~App();

	void Run();


private:

	void Events();

	void Options();

	void ImGUI();

	void Update();

	void Render();

	void EndFrame();


private:

	bool End = 0;

	bool Drag = 0;
	bool selectSquare = 0;
	// bool wireBool = 0;
	
	bool stimuliDisplay = 1;
	bool stimuliEndNodes = 0;
	
	bool Selection = 0;
	bool releaseBool = 1;
	bool ShiftPressed = 0;
	bool mouseOnCompsBool = 0;
	
	bool MInTool = 0;
	bool MIntool = 0;
	bool cursorInWin = 0;
	float t_TollWx = 0;
	
	int serialToolMouse = 0;
	//int serialCompMouse = 0;
	
	time_t FrameTime_for_FPS = 0;
	//bool PlayMode = 0; //asdf
	//bool PlayRot = 0;  //asdf

	// ImGui
	sf::Clock deltaClock;

	// Solver
	Graph circuit;
};