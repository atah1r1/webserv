#include "Utils.hpp"

const char *getFileExtension(const std::string& fileName) {
	int index = fileName.find_last_of(".");
	if (index != -1) {
		std::string ext = fileName.substr(index + 1);
		return ext.c_str();
	}
	return "";
}

bool beginsWith(const std::string& haystack, const std::string& needle) {
	if (haystack.empty() || needle.empty()) return false;
	std::string::const_iterator hit = haystack.begin();
	std::string::const_iterator nit = needle.begin();
	while (nit != needle.end())
	{
		if (*nit != *hit) return false;
		++nit; ++hit;
	}
	return true;
}

const std::string trim(const std::string& str) {
	if (str.empty()) return str;
	size_t _begin = 0;
	size_t _end = str.length() - 1;
	while (_begin < str.length() && str[_begin] == ' ') ++_begin;
	while (_end > _begin && str[_end] == ' ') --_end;
	return str.substr(_begin, _end - _begin + 1);
}

std::pair<size_t, std::string> nextLine(const std::string& str, size_t start) {
	if (str.empty()) return std::make_pair(0, str);
	size_t _end = start;

	while (_end < str.length() && str[_end] != '\n') ++_end;
	std::string _line = str.substr(start, _end - start);

	if (_end < str.length())
		return std::make_pair(++_end, _line);
	return std::make_pair(_end, _line);
}

Location *matchLocation( std::vector<Location *> locations, const std::string& path) {
	for (size_t i = 0; i < locations.size(); ++i) {
		if (locations[i] == NULL) continue;

		std::string _loc = locations[i]->_location;
		if (_loc.back() == '/') {
			// remove last "/"
			_loc = _loc.substr(0, _loc.size() - 1);
		}
		// check if path startswith or equals location
		if (beginsWith(path, _loc)) {
			return locations[i];
		}
	}
	return NULL;
}
