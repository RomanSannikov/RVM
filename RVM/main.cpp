#include <iostream>
#include <array>

#include "../include/argh/argh.h"

#include "scanner.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "VM.hpp"


std::string filename;

enum programMode
{
	BINARY_OUTPUT = 0,
	COMPILE
};


void startVirtualMachine(const std::string& c_filename, const std::vector<programMode>& c_programMode)
{	
	Scanner scanner(c_filename);
	Tokenizer tokenizer;
	Parser parser;
	VM vm;
	
	std::string lineFromScanner;
	
	// Fix: !scanner.isEOF() && compile == true
	while (!scanner.isEOF())
	{
		scanner.getLine(lineFromScanner);

		if (lineFromScanner.empty())
			continue;

		std::cout << lineFromScanner << std::endl;
		
		tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());

		parser.parse(tokenizer.tokens);
	}

	parser.completeParsing();
	parser.printInstructions(); // Fix: it's temporary

	vm.run(parser.getInstructions());
}


int main(int argc, char* argv[])
{
	argh::parser cmdl(argv);
	std::vector<programMode> programMode;

	if (cmdl[{"-b", "--binary"}])
		programMode.emplace_back(programMode::BINARY_OUTPUT);
	if (cmdl[{"-o", "--output"}] ? "ON" : "OFF")
		programMode.emplace_back(programMode::COMPILE);

	cmdl({"-f", "--filename"}) >> filename;

	startVirtualMachine(filename, programMode);

	system("pause");
	return 0;
}  