#include <cassert>
#include <bitset>
#include <span>
#include <ranges>
#include <string_view>

#include "scanner.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "VM.hpp"


using bitmode = std::bitset<8>;
std::string filename;


enum programMode
{
	RUN_BINARY = 0,
	MAKE_OUTPUT
};


static void parseArguments(bitmode& mode, const auto c_arguments)
{
	auto isArgument = [](auto& argument, std::string_view shortName, std::string_view longName)
	{
		return (!shortName.empty() && *argument == shortName) || *argument == longName;
	};
	auto next = [&c_arguments](auto& iter)
	{
		iter++;
		if (iter == c_arguments.end())
			throw RVMError("cannot recognize command line arguments");
	};

	for (auto c_i = c_arguments.begin() + 1; c_i != c_arguments.end(); c_i++)
	{
		if (isArgument(c_i, "-b", "--binary"))
		{
			mode.set(programMode::RUN_BINARY);
		}
		else if (isArgument(c_i, "-o", "--output"))
		{
			next(c_i);
			if (*c_i == "ON") mode.set(programMode::MAKE_OUTPUT);
		}
		else if (isArgument(c_i, "-f", "--filename"))
		{
			next(c_i);
			filename = *c_i;
		}
		else
		{
			throw RVMError("cannot recognize command line arguments");
		}
	}
}


static void startVirtualMachine(const std::string& c_filename, const bitmode& c_mode)
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

			Logger::print(lineFromScanner);

			tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());

			parser.parse(tokenizer.tokens);
		}

		parser.completeParsing();
		Logger::printInstructions(parser.getInstructions());
	}
	else
	{
		scanner.open(c_filename);
		parser.loadInstructions(scanner.readBinary());
	}

	if (c_mode.test(programMode::MAKE_OUTPUT) && !c_mode.test(programMode::RUN_BINARY))
		parser.outputInstructions(c_filename);
	else
		vm.run(parser.getInstructions());
}


int main(int argc, char* argv[])
{
	bitmode mode;
	const auto c_args = std::span(argv, argc) | std::views::transform([](const char* str) { return std::string_view{str}; });
	try {
		parseArguments(mode, c_args);
		startVirtualMachine(filename, mode);
	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}  