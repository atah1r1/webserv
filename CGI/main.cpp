#include "CGI.hpp"

int main(int ac, char **av, char **env)
{
	std::string CGI_PATH = "./bin/PHP_CGI";
	std::string FILE_PATH = "./tests/test.php";
	std::string result = CGI::execute(CGI_PATH, FILE_PATH, env);
	std::cout << result << std::endl;
	return 0;
}