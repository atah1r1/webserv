#include "Debug.hpp"

const char *getLevelStr(Level l) {
	switch (l)
	{
	case _DEBUG:
		return C_BLUE "Debug: ";
	case _ERROR:
		return C_RED "Error: ";
	case _INFO:
		return C_GREEN "Info: ";
	default:
		return "Other: ";
	}
}

void debugPrint(Level level, const char *file, const int line, const std::string& message) {
	#if defined(ALLOW_DEBUG)
		std::cerr << getLevelStr(level) << file << ": " << line << ": " << message << C_RESET << std::endl;
	#endif
}
