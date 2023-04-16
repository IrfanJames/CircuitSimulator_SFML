#include "LOG.hpp"

namespace LOG {

#ifdef NDEBUG
	std::ofstream log_file;
#endif

	void initializeLOG() {

#ifdef NDEBUG
		std::remove("log.txt");
		log_file.open("log.txt", std::ios::out, std::ios::trunc);
#endif
	}
}
