#include <iostream>
#include <map>
#include "zsearch.h"

int main () {
	jlu::ZSearch zs;
	zs.setPath ("test/asserts/");
	// std::multimap<int, std::string> info = zs.search ("Mayonesa con patatas #alimentación");
	std::multimap<int, std::string> info = zs.search ("Mayonesa con patatas #alimentación");

	std::cout << "--------\n" << std::endl;

	for (auto it = info.begin (); it != info.end (); it++) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
	return EXIT_SUCCESS;
}
