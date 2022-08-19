#include "../Config/ServerConfig.hpp"
#include "../CGI/cgi.hpp"
#include "../Includes/Webserv.hpp"
#include "../Request/Request.hpp"
#include "../Request/Utils.hpp"
#include "../Response/Response.hpp"
#include "../Response/ResponseHandler.hpp"
#include "../Utils/FileHandler.hpp"
#include "../Utils/StatusCodes.hpp"
#include "../Utils/Utils.hpp"

int main(int ac, char **av) {
	// create fake config;
	ServerConfig conf;
	Location loc1;

	conf.setServerName("www.example.com");
	conf.setServerIp("0.0.0.0");
	conf.setPort(8080);
	conf.setRoot("/Users/ehakam/Desktop/webserv");
	conf.setErrorPages(std::make_pair<int, std::string>(404, "/Tests/Errors/error_404.html"));

	loc1._allow_methods.push_back("GET");
	loc1._allow_methods.push_back("POST");
	loc1._allow_methods.push_back("DELETE");
	loc1._allow_methods.push_back("PUT");
	loc1._autoindex = true;
	//loc1._index_file.push_back("index.html");
	loc1._index_file.push_back("index.php");
	loc1._location = "/Tests/";

	conf.setLocations(&loc1);

	std::vector<ServerConfig *> co;
	co.push_back(&conf);

	// create request
	Request req;

	req.setMethod("GET");
	req.setPath("/Tests/Home/file.php");
	req.setPort(8080);
	req.setHost("0.0.0.0");
	req.setHeader(H_HOST, "0.0.0.0");
	req.setHeader(H_CONTENT_LENGTH, "0");
	req.setVersion(HTTP_VERSION);

	// now test the stuff
	Response r = ResponseHandler::handleRequests(req, co);

	//std::cout << r.toString() << std::endl;

	return 0;
}

