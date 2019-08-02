#include "scanner.hpp"


inline void Scanner::open(const std::string& filename) { file.open(filename); }


inline void Scanner::open(const char* const filename) { file.open(filename); }


inline bool Scanner::isEOF() { return file.eof(); }


inline bool Scanner::isOpen() { return file.is_open(); }


std::string Scanner::getLine()
{
	if (!file.is_open())
		throw std::exception("Exception from scanner");

	char* scannedLine = (char*)malloc(c_lineSize);
	file.getline(scannedLine, c_lineSize);

	return scannedLine;
}