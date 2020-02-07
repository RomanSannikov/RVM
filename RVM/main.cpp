#include <iostream>
#include <array>
#include <bitset>

#include "../include/argh/argh.h"

#include "scanner.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "VM.hpp"


enum programMode
{
	RUN_BINARY = 0,
	MAKE_OUTPUT
};


void startVirtualMachine(const std::string& c_filename, const std::bitset<8>& c_mode)
{	
	Scanner scanner;
	Tokenizer tokenizer;
	Parser parser;
	VM vm;
	
	std::string lineFromScanner;
	
	if (!c_mode.test(programMode::RUN_BINARY))
	{
		scanner.open(c_filename);

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
	}
	else
	{
		scanner.open(c_filename);
		parser.loadInstructions(scanner.readBinary());
	}

	if (c_mode.test(programMode::MAKE_OUTPUT) && !c_mode.test(programMode::RUN_BINARY))
		parser.outputInstructions(c_filename);

	vm.run(parser.getInstructions());
}


int main(int argc, char* argv[])
{
	argh::parser cmdl(argv);
	std::string filename;
	std::bitset<8> mode;

	if (cmdl[{"-b", "--binary"}])
		mode.set(programMode::RUN_BINARY);
	if (cmdl[{"-o", "--output"}] ? "ON" : "OFF")
		mode.set(programMode::MAKE_OUTPUT);

	cmdl({"-f", "--filename"}) >> filename;

	startVirtualMachine(filename, mode);

	system("pause");
	return 0;
}  