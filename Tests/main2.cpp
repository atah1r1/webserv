#include <iostream>
#include <cstdio>

int main(int ac, char **av) {
	if (av[1][0] == '.') {
		std::cerr << "Ach kaddiir assi: " << av[1] << std::endl;
		return 1;
	}
	int i = remove(av[1]);
	std::cerr << strerror(errno) << std::endl;
	return 0;
}