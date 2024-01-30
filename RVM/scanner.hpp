#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <cassert>

#include "error.hpp"
#include "instruction.hpp"

class Scanner
{
private:
	std::ifstream file;
	unsigned lineNumber;

public:
	Scanner() {}
	Scanner(const std::string& c_filename) : file(c_filename) {}
	Scanner(const char* const c_filename) : file(c_filename) {}

	~Scanner() { file.close(); }
	
	void open(const std::string&);
	void open(const char* const);
	
	bool isEOF();

	std::string getLine(std::string&);
	unsigned getLineNumber();

	const std::vector<instructionType> readBinary();
};