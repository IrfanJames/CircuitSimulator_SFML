#pragma once

#include <fstream>

namespace LOG {

	// Once in any file
	void initializeLOG();

#ifdef NDEBUG
	// Making .txt File For Logging in Release Mode
	extern std::ofstream log_file;
#endif
}

// LOG Function
#ifdef _DEBUG
#define LOG(x) std::cout << x;
#else
#define LOG(x) LOG::log_file  << x;
#endif

// LOG_VEC Function
#define LOG_VEC(vec) LOG("\n(" << vec.size() << "): "); for (size_t i = 0; i < vec.size(); i++) LOG(vec[i] << " "); LOG("\n");
