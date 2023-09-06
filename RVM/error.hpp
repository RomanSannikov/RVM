#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include "exceptions.hpp"


inline void printErrorAndExit(const std::string& c_message) noexcept
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl 
		<< "ERROR MESSAGE: " << c_message << std::endl;
	exit(EXIT_FAILURE);
}


inline void printErrorAndExit(const std::string& c_message, const unsigned c_lineNumber) noexcept
{
	std::cout << std::endl << "ERROR APPEARED!" << std::endl
		<< "ERROR MESSAGE: " << c_message << std::endl 
		<< "LINE: " << c_lineNumber << std::endl;
	exit(EXIT_FAILURE);
}