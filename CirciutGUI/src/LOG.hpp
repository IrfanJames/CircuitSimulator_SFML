#pragma once

#include <iostream>
#include <fstream>

namespace LOG {

#ifdef NDEBUG
	// Making .txt File For Logging in Release Mode
	inline std::ofstream log_file("temp_files/log.txt", std::ios::out, std::ios::trunc);
#endif

}

// LOG Function
#ifdef _DEBUG
#define LOG(x) std::cout << x;
#else
#define LOG(x) LOG::log_file << x;
#endif

// LOG_VEC Function
#define LOG_VEC(vec) LOG("\n(" << vec.size() << "): "); for (size_t i = 0; i < vec.size(); i++) LOG(vec[i] << " "); LOG("\n");

// LOG_VEC Function
#define LOG_VEC2(vec) LOG("(" << vec.x << ", " << vec.y << "), ");
