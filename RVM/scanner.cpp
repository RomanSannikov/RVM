#include "scanner.hpp"


void Scanner::open(const std::filesystem::path& c_filename) { file.open(c_filename); }


bool Scanner::isEOF() { return file.eof(); }


std::string Scanner::getLine(std::string& scannedLine)
{
	if (!file.is_open())
		throw ScannerError("The file is not open!");

	std::getline(file, scannedLine);
	++lineNumber;

	return scannedLine;
}


const std::vector<instructionType> Scanner::readBinary()
{
	if (!file.is_open())
		throw ScannerError("The file is not open!");

	std::string scannedLine;
	std::vector<instructionType> instructions;

	std::getline(file, scannedLine);

	assert(scannedLine.size());

	for (unsigned i = 0; i < scannedLine.size(); i += 8)
	{
		const auto binary_sequence = scannedLine.substr(i, 8);
		if (!std::all_of(binary_sequence.begin(), binary_sequence.end(), [](const auto& ch) { return ch == '0' || ch == '1'; }))
			throw ScannerError("The binary file contains non-binary symbols");
		instructions.emplace_back(static_cast<instructionType>(std::bitset<sizeof(instructionType) * 8>(binary_sequence).to_ulong()));
	}

	return instructions;
}


unsigned Scanner::getLineNumber() { return lineNumber; }