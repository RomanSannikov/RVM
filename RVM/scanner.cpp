#include "scanner.hpp"


void Scanner::open(const std::string& c_filename) { file.open(c_filename); }


void Scanner::open(const char* const c_filename) { file.open(c_filename); }


bool Scanner::isEOF() { return file.eof(); }


std::string Scanner::getLine(std::string& scannedLine)
{
	if (!file.is_open())
		throw RVMError(c_scannerError + "The file is not open!");

	std::getline(file, scannedLine);
	++lineNumber;

	return scannedLine;
}


const std::vector<instructionType> Scanner::readBinary()
{
	if (!file.is_open())
		throw RVMError(c_scannerError + "The file is not open!");

	std::string scannedLine;
	std::vector<instructionType> instructions;

	std::getline(file, scannedLine);

	assert(scannedLine.size());

	for (unsigned i = 0; i < scannedLine.size(); i += 8)
		instructions.emplace_back((instructionType)std::bitset<sizeof(instructionType) * 8>(scannedLine.substr(i, 8)).to_ulong());

	return instructions;
}


unsigned Scanner::getLineNumber() { return lineNumber; }