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

inline void printErrorAndExit(const std::string& c_message, const unsigned c_lineNumber)
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl
		<< "ERROR MESSAGE: " << c_message << std::endl 
		<< "LINE: " << c_lineNumber << std::endl;

	system("PAUSE");
	exit(EXIT_FAILURE);
}