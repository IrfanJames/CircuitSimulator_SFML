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
#ifdef MY_NDIST

#ifdef _DEBUG
#define LOG(x) std::cout << x;
#else
#define LOG(x) LOG::log_file << x;
#endif

#else
#define LOG(x) ;
#endif

// LOG_VEC Function
#ifdef MY_NDIST
#define LOG_VEC(vec) LOG("\n(" << vec.size() << "): "); for (size_t i = 0; i < vec.size(); i++) LOG(vec[i] << " "); LOG("\n");
#else
#define LOG_VEC(vec) ;
#endif

// LOG_VEC Function
#ifdef MY_NDIST
#define LOG_VEC2(vec) LOG("(" << vec.x << ", " << vec.y << "), ");
#else
#define LOG_VEC2(vec) ;
#endif