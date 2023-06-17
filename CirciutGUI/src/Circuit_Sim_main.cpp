
/*
	* Colapse all Scopes - it'll be easier
	* Ctrl + M + A in Visual Studio
	

taskkill /F /IM CirciutGUI.exe
*/

#include "Circuit_App.hpp"

#define NOMINMAX // SFML RectInt.inl error // https://en.sfml-dev.org/forums/index.php?topic=26401.0
#include <windows.h>


#ifdef _DEBUG
// Debug  Mode
int main(int argc, char** argv) {

	// Dropped Files
	std::vector<std::string> filePaths;
	{
		for (int i = 1; i < argc; i++)
			filePaths.emplace_back(argv[i]);
	}

	App* CircuitSim = new App(filePaths);

	CircuitSim->Run();

	delete CircuitSim;

	//std::cin.get();
}

#endif


#ifdef NDEBUG
// Realse Mode
// Dist Mode

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// Dropped Files
	std::vector<std::string> filePaths;
	{
		int argc;
		LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

		for (int i = 1; i < argc; i++) {

			std::wstring file_wstr(argv[i]);
			std::string file_str(file_wstr.begin(), file_wstr.end());

			filePaths.emplace_back(file_str);

			//LOG("\n" << i << ": " << filePaths.back());
		}
		LocalFree(argv);
	}

	App* CircuitSim = new App(filePaths);

	CircuitSim->Run();

	delete CircuitSim;

	//return 0;
}
#endif