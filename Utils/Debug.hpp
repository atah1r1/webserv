
#ifndef __DEBUG_HPP__
# define __DEBUG_HPP__

# include <iostream>

#define C_RED "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_BLUE "\033[1;34m"
#define C_RESET "\033[0m"

enum Level {
	_DEBUG = 100,
	_ERROR = 200,
	_INFO = 300,
};

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

inline void debugPrint(Level level, const char *file, const int line, const std::string& message) {
	#if defined(ALLOW_DEBUG)
		std::cerr << getLevelStr(level) << file << ": " << line << ": " << message << C_RESET << std::endl;
	#endif
}

#endif