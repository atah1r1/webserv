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

	return 0;
}

