#include "scanner.hpp"


void Scanner::open(const std::string& c_filename) { file.open(c_filename); }


void Scanner::open(const char* const c_filename) { file.open(c_filename); }


bool Scanner::isEOF() noexcept { return file.eof(); }


std::string Scanner::getLine(std::string& scannedLine) noexcept
{
	if (!file.is_open())
		throw RVMError(c_scannerError + "The file is not open!");

	std::getline(file, scannedLine);
	++lineNumber;

	return scannedLine;
}


const std::vector<int8_t> Scanner::readBinary()
{
	if (!file.is_open())
		throw RVMError(c_scannerError + "The file is not open!");

	std::string scanndedLine;
	std::vector<int8_t> instructions;

	std::getline(file, scanndedLine);

	assert(scanndedLine.size());

	for (unsigned i = 0; i < scanndedLine.size(); i += 8)
		instructions.emplace_back((int8_t)std::bitset<8>(scanndedLine.substr(i, 8)).to_ulong());

	return instructions;
}


unsigned Scanner::getLineNumber() { return lineNumber; }