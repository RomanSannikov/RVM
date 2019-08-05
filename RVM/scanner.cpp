#include "scanner.hpp"


inline void Scanner::open(const std::string& c_filename) { file.open(c_filename); }


inline void Scanner::open(const char* const c_filename) { file.open(c_filename); }


inline bool Scanner::isEOF() { return file.eof(); }


inline bool Scanner::isOpen() { return file.is_open(); }


std::string Scanner::getLine(std::string& scannedLine)
{
	if (!file.is_open())
		printErrorAndExit(c_scannerError + "Cannot open the file!");

	std::getline(file, scannedLine);

	return scannedLine;
}