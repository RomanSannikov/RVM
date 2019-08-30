#include "scanner.hpp"


inline void Scanner::open(const std::string& c_filename) { file.open(c_filename); }


inline void Scanner::open(const char* const c_filename) { file.open(c_filename); }


bool Scanner::isEOF() noexcept { return file.eof(); }


std::string Scanner::getLine(std::string& scannedLine) noexcept
{
	if (!file.is_open())
		printErrorAndExit(c_scannerError + "The file is not open!");

	std::getline(file, scannedLine);
	++lineNumber;

	return scannedLine;
}


unsigned Scanner::getLineNumber() { return lineNumber; }