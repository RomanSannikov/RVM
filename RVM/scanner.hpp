#pragma once

#include <string>
#include <fstream>

#include "error.hpp"

class Scanner
{
private:
	const unsigned int c_lineSize = 128;
	std::ifstream file;

public:
	Scanner(const std::string& filename) : file(filename) {}
	Scanner(const char* const filename) : file(filename) {}
	
	inline void open(const std::string&);
	inline void open(const char* const);
	
	inline bool isEOF();
	inline bool isOpen();

	std::string getLine();
};