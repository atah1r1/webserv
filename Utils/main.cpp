
#include "Utils.hpp"

int main(int ac, char **av) {
	Location l;
	l._location = "/wordpress/config";
	Location l2;
	l2._location = "/phpmyadmin/";

	std::vector<Location *> v;

	v.push_back(&l2);
	v.push_back(&l);

	Location* x = matchLocation(v, "/phpmyadmin/config/home/");

	if (x != NULL) {
		std::cout << "YAAAY" << std::endl;
	} else {
		std::cout << "DAAMN" << std::endl;
	}

	return (0);
}