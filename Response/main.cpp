// #include "Response.hpp"
// #include "ResponseHandler.hpp"
// #include "../Request/Request.hpp"
// #include "../Config/ServerConfig.hpp"
// #include "../Utils/FileHandler.hpp"
// #include "../Utils/Utils.hpp"
// #include <unistd.h>
// #include <iostream>
// #include <dirent.h>
// #include <sys/stat.h>

// #define BUFFER_SIZE 1024

// bool getNextChunk(std::fstream& _file, char *buffer) {
// 	if (_file.good()) {
// 		debugPrint(_ERROR, __FILE__, __LINE__, "Reponse: getChunk: _file is bad");
// 		return false;
// 	}
// 	if (_file.eof()) return false;
// 	_file.read(buffer, BUFFER_SIZE - 1);
// 	return true;
// }

// int main(int ac, char **av) {

// 	std::fstream _file;

// 	_file.open(av[1], std::fstream::binary );

// 	if (!_file.is_open()) {
// 		std::cerr << "Error opening file" << std::endl;
// 	}

// 	char buffer[BUFFER_SIZE] = {0};

// 	while(getNextChunk(_file, buffer)) {
// 		std::cerr << "============= CHUNK BEGIN" << std::endl;
// 		std::cerr << buffer << std::endl;
// 		std::cerr << "============= CHUNK END" << std::endl;
// 	}

// 	_file.close();
	//explore(av[1], "", av[2]);

	// Request r;
	// r.setMethod("POST");
	// r.setPath("/cgi-l");
	// r.setVersion(HTTP_VERSION);
	// r.setHost("www.tutorialspoint.com");

	// r.setHeader("Host", "www.tutorialspoint.com");
	// r.setHeader("Content-Type", "application/x-www-form-urlencoded");
	// r.setHeader("Content-Length", "20");

	// Location l;
	// l._location = "/cgi-bin";
	// Location l2;
	// l2._location = "/cgi-bin/pro6/";
	// Location l3;
	// l3._location = "/cgi-bin/pro6/wow/";
	// Location l4;
	// l4._location = "/";

	// ServerConfig sc;
	// sc.setServerName("www.tutorialspoint.com");
	// sc.setLocations(&l);
	// sc.setLocations(&l2);
	// sc.setLocations(&l3);
	// sc.setLocations(&l4);

	// Location l2;
	// l2._location = "/wordpress/";

	// ServerConfig sc2;
	// sc2.setServerName("www.google.com");
	// sc2.setLocations(&l2);

	// std::vector<ServerConfig *> sr;

	// sr.push_back(&sc);

	// std::pair<ServerConfig *, Location *> p = ResponseHandler::getConfig(r, sr);

	// if (p.first != NULL) {
	// 	std::cout << "SERVERNAME: " << p.first->getServerName() << std::endl;
	// } else {
	// 	std::cout << "SERVERNAME: " << "SHIIIIT" << std::endl;
	// }

	// if (p.second != NULL) {
	// 	std::cout << "LOC: " << p.second->_location << std::endl;
	// } else {
	// 	std::cout << "LOC: " << "SHIIIIT" << std::endl;
	// }

	// std::cout << "|" << toHeaderCase("CONtent") << "|" << std::endl;
	return (0);
}