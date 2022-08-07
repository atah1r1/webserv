
#include "Utils.hpp"
#include "FileHandler.hpp"

#define OUT std::cout
#define EN std::endl
#define STR std::string

template <typename K, typename V>
void printMap(std::map<K, V> m) {
	typename std::map<K, V>::iterator it = m.begin();

	while (it != m.end())
	{
		OUT << std::setw(20) << std::left << it->first << " = " << it->second << EN;
		++it;
	}
}

void UTILS_TEST(int ac, char **av) {
	std::cout << std::boolalpha;
	// test getCGIPath
	std::cout << "CGI: " << getCGIPath(av[1]) << std::endl;

	std::cout << "UPPER: [" << toUpperCase(av[1]) << "]" << std::endl;

	std::cout << "LOWER: [" << toLowerCase(av[1]) << "]" << std::endl;

	std::string marr[] = {"GET", "POST"};

	std::vector<std::string> v(marr, marr + 3);

	std::cout << "IS ALLOWED: (" << av[1] << ") [" << isMethodAllowed(v, av[1]) << "]" << std::endl;

	std::cout << "IS IMPL: (" << av[1] << ") [" << isMethodImplemented(av[1]) << "]" << std::endl;

	std::cout << "to NUMBER: " << toNumber<int>(av[1]) << std::endl;
	std::cout << "to STR: " << toString<int>(234) << std::endl;
	std::cout << "to STR: " << toString<int>(0) << std::endl;
	std::cout << "to STR: " << toString<int>(-12) << std::endl;
}

void FILE_HANDLER_TEST(int ac, char **av) {
	OUT << std::boolalpha;
	// std::string x = FileHandler::readFile(av[1]);
	// std::cout << x << std::endl;

	//
	// OUT << "FULL PATH: [" << FileHandler::getFullPath(av[1], av[2]) << "]" << EN;

	// STR f = FileHandler::getFullPath(av[1], av[2]);

	// OUT << "DISC PATH: [" << FileHandler::disconnectPath(av[1], f) << "]" << EN;

	//OUT << "Mod date: [" << FileHandler::getLastModificationDate(av[1]) << "]" << EN;

	// std::string iarr[] = {"index.htm", "index.php", "index.html"};
	// std::vector<std::string> iv(iarr, iarr + 3);

	// STR p = FileHandler::searchIndexes(av[1], iv);

	// OUT << "IDX PATH: [" << p << "]" << EN;
	// std::map<STR, STR> m = FileHandler::exploreDir(av[1], FileHandler::getFullPath(av[1], av[2]));

	// perror("Error");
	// std::cerr << "errno: " << errno << EN;

	// printMap(m);
	FileType t = FileHandler::getTypeS(av[1]);

	perror("Error");
	std::cerr << "errno: " << errno << EN;

	switch (t) {
	case T_FILE:
		OUT << "T_FILE" << EN;
		break;
	case T_DIR:
		OUT << "T_DIR" << EN;
		break;
	case T_OTHER:
		OUT << "T_OTHER" << EN;
		break;
	case T_LINK:
		OUT << "T_LINK" << EN;
		break;
	case T_ERROR:
		OUT << "T_ERROR" << EN;
		break;
	default:
		break;
	}
	// bool x = FileHandler::pathExists(av[1]);
	// OUT << "Exists: " << x << EN;
}
// ERRNO
// 20 Not a dir
// 13 Permission denied
// 62 Too many levels of symbolic links


int main(int ac, char **av) {

	//UTILS_TEST(ac, av);
	//FILE_HANDLER_TEST(ac, av);

	std::string path = std::string("./Test_delete/") + av[1];

	//OUT << "PATH: " << path << EN;

	bool b = FileHandler::removeAll(path);

	if (!b) std::cerr << "END: " << strerror(errno) << std::endl;

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

	// Response r;

	// r.setVersion("HTTP/1.1");
	// r.setStatusCode(200);
	// r.setStatus("OK");
	// r.setBody("<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>");

	// r.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT");
	// r.addHeader("Server", "Webserv/0.0.1 (MacOS)");
	// r.addHeader("Last-Modified", "Wed, 22 Jul 2009 19:15:56 GMT");
	// r.addHeader("Content-Length", "88");
	// r.addHeader("Content-Type", "text/html");
	// r.addHeader("Connection", "Closed");

	// std::cout << r.toString() << std::endl;

	// Response r2 = Response::parseFrom(r.toString());


	return (0);
}
