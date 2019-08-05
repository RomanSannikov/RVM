#pragma once

#include <string>
#include <fstream>

#include "error.hpp"

class Scanner
{
private:
	const unsigned c_lineSize = 128;
	std::ifstream file;

public:
	Scanner(const std::string& c_filename) : file(c_filename) {}
	Scanner(const char* const c_filename) : file(c_filename) {}

	~Scanner() { file.close(); }
	
	inline void open(const std::string&);
	inline void open(const char* const);
	
	inline bool isEOF();
	inline bool isOpen();

	std::string getLine(std::string&);
};