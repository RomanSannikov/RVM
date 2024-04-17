#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <cassert>
#include <filesystem>

#include "exceptions.hpp"
#include "instruction.hpp"

class Scanner
{
private:
	std::ifstream file;
	unsigned lineNumber = 0;

public:
	Scanner() = default;
	explicit Scanner(const std::filesystem::path& c_filename) : file(c_filename) {}

	~Scanner() { file.close(); }
	
	void open(const std::filesystem::path&);
	
	bool isEOF();

	std::string getLine(std::string&);
	unsigned getLineNumber();

	const std::vector<instructionType> readBinary();
};