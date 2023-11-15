#pragma once

#include <chrono>
#include "LOG.hpp"

class Timer {

public:
	
	Timer(const std::string& name = "UnNamed") {
		mTimer = std::chrono::high_resolution_clock::now();
		mName = name;
	}
	~Timer() {
		double dur = duration();
		LOG("\n" << mName << ": " << dur);
		
		if (!mName.empty() && mName.back() == '\n')
			LOG("\n__________________\n");
	}

	double duration() {
		static std::chrono::duration<double> duration;

		duration = std::chrono::high_resolution_clock::now() - mTimer;
		return duration.count() * 1000;
	}

	void reset() {
		mTimer = std::chrono::high_resolution_clock::now();
	}

	bool repeatEvery(double milliSec) {
		if (milliSec <= duration()) {
			reset();
			return 1;
		}
		else return 0;
	}

private:
	std::string mName;
	std::chrono::steady_clock::time_point mTimer;
};