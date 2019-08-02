#include <iostream>
#include <string>
#include <cstdlib>

#include "exceptions.hpp"

void error(const std::string& message)
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl 
		<< "ERROR MESSAGE: " << message << std::endl;
	exit(EXIT_FAILURE);
}

void error(const char* const message)
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl
		<< message << std::endl;
	exit(EXIT_FAILURE);
}