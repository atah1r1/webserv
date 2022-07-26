
#include "../Response/Response.hpp"

int main(int ac, char **av) {
	// Location l;
	// l._location = "/wordpress/config";
	// Location l2;
	// l2._location = "/phpmyadmin/";

	// std::vector<Location *> v;

	// v.push_back(&l2);
	// v.push_back(&l);

	// Location* x = matchLocation(v, "/phpmyadmin/config/home/");

	// if (x != NULL) {
	// 	std::cout << "YAAAY" << std::endl;
	// } else {
	// 	std::cout << "DAAMN" << std::endl;
	// }

	// std::string s = "  f   f   ";

	// std::cout << "|" << s << "|" << std::endl;

	// std::string t = trim(s);

	// std::cout << "|" << t << "|" << std::endl;

	Response r;

	r.setVersion("HTTP/1.1");
	r.setStatusCode(200);
	r.setStatus("OK");
	r.setBody("<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>");

	r.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT");
	r.addHeader("Server", "Webserv/0.0.1 (MacOS)");
	r.addHeader("Last-Modified", "Wed, 22 Jul 2009 19:15:56 GMT");
	r.addHeader("Content-Length", "88");
	r.addHeader("Content-Type", "text/html");
	r.addHeader("Connection", "Closed");

	std::cout << r.toString() << std::endl;

	Response r2 = Response::parseFrom(r.toString());

	std::cout << r2.toString() << std::endl;

	return (0);
}