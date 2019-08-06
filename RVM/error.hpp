#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include "exceptions.hpp"

inline void printErrorAndExit(const std::string& c_message)
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl 
		<< "ERROR MESSAGE: " << c_message << std::endl;

	system("PAUSE");
	exit(EXIT_FAILURE);
}