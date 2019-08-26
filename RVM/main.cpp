#include <iostream>

#include "scanner.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "VM.hpp"


void startVirtualMachine()
{
	const std::string c_filename = "simpleTest.txt"; // Fix: temporary value
	
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
	vm.printStack();
}


int main(int argc, char* argv[])
{
	startVirtualMachine();

	system("pause");
	return 0;
}  