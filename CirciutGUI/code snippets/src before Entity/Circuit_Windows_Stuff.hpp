// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
//
// All Windows APIs used in Project
#pragma once

#include <iostream>
#include <windows.h>

std::string OpenFileDialog(const char* filter, int tempNumber = 0)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Project-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];
	//---------------


	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	//ofn.hwndOwner = CircuitGUI::app.getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string SaveFileDialog(const char* filter, int tempNumber = 0)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	//---------------
	std::string tn = "Untitled-" + std::to_string(tempNumber);
	for (int c = 0; c < tn.size(); c++)
		szFile[c] = (int)tn[c];
	//---------------

	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE) {
		/*LPCWSTR temp = (wchar_t*)ofn.lpstrFile;
		ShellExecute(NULL, NULL, temp, NULL, NULL, SW_SHOWNORMAL);*/
		return ofn.lpstrFile;
	}

	return std::string();
}

void wintest() {
	std::cout <<
		MessageBoxW(
			NULL,
			L"Holoy Mooly\nYou Pressed 'M'",
			L"My 1st Box",
			MB_YESNOCANCEL | MB_ICONEXCLAMATION);
}