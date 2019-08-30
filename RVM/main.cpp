#include <iostream>
#include <array>

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


const std::vector<programMode>& parseArguments(const int& c_numberOfArguments, const char* c_arguments[])
{
	const std::array<const std::string, 3> c_args = { "-f", "-b", "-o" };
	std::vector<programMode> programMode;

	for (int i = 1; i < c_numberOfArguments; ++i)
	{
		for (const auto& j : c_args)
		{
			if (!j.compare(c_arguments[i]))
			{
				if (i + 1 < c_numberOfArguments)
				{
					if (filename.empty())
					{ filename = c_arguments[i + 1]; ++i; }
					else printErrorAndExit(c_argumentParserError + "filename has already been defined");

				}
				else printErrorAndExit(c_argumentParserError + "there are no filename arter " + j);
			}
		}
	}

	return programMode;
}


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
	//std::vector<programMode>&& programMode = std::move(parseArguments(argc, argv));
	std::vector<programMode> programMode;
	startVirtualMachine("simpleTest.txt", programMode);

	system("pause");
	return 0;
}  