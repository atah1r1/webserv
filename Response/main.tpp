// #include "Response.hpp"
// #include "ResponseHandler.hpp"
// #include "../Request/Request.hpp"
// #include "../Config/ServerConfig.hpp"
//#include "../Utils/FileHandler.hpp"
#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

/*
POST /cgi-bin/process.cgi HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Content-Type: application/x-www-form-urlencoded
Content-Length: length
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive

licenseID=string&content=string&/paramsXML=string
*/

void explore(const char *dir_name, const std::string& indent, const std::string& ext){
    DIR *dir; // pointer to directory
    struct dirent *entry; // all stuff in the directory
    struct stat info; // info about each entry

    dir = opendir(dir_name);

    if (!dir)
    {
        std::cout << "Directory not found" << std::endl;
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            std::string path = std::string(dir_name) + "/" + std::string(entry->d_name);
			stat(path.c_str() ,&info);

			// if (FileHandler::getFileExtension(path) == ext)
            // 	std::cout << path << std::endl;

            if (S_ISDIR(info.st_mode) && !S_ISLNK(info.st_mode))
                explore(path.c_str(), indent + "    ", ext);
        }
    }
    closedir(dir);
}

inline bool fileExists( const std::string& path ) {
	return ( access( path.c_str(), F_OK ) != -1 );
}

int main(int ac, char **av) {

	std::cout << std::boolalpha;

	std::cout << fileExists(std::string(av[1])) <<std::endl;

	//explore(av[1], "", av[2]);

	// Request r;
	// r.setMethod("POST");
	// r.setPath("/cgi-l");
	// r.setVersion("HTTP/1.1");
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