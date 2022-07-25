
#include <iostream>
#include "../Config/ServerConfig.hpp"
#include "Debug.hpp"

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

