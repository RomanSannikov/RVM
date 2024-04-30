#include <cassert>
#include <bitset>
#include <span>
#include <ranges>
#include <string_view>
#include <filesystem>
#include <utility>

#include "parser.hpp"
#include "VM.hpp"


using bitmode = std::bitset<8>;
std::filesystem::path filename;

const std::string_view c_help_message =
R"(RVM - Rolang Virtual Machine

DESCRIPTION
  RVM is a virtual machine that runs Rolang ByteCode both in binary and text forms.

USAGE:
  RVM [submodule] [options] <file>

SUBMODULES:
  run            run the code from the file (this is a default submodule)
  convert        convert the provided file from one format to another

OPTIONS:
  -b, --binary   hint that the provided file is in the rbc format
  -h, --help     print help information

EXAMPLE OF ROLANG BYTECODE:
  pushn 2
  pushn 1
  add
  popn
  hlt

GitHub repository for more information: https://github.com/RomanSannikov/RVM
)";


enum programSubmodule
{
	NONE_SUBMODULE = 0,
	EXECUTOR,
	CONVERTOR
};

enum submoduleMode
{
	BINARY = 0
};


static std::pair<programSubmodule, bitmode> parseArguments(const auto& c_arguments)
{
	auto isArgument = [](const auto& argument, std::string_view shortName, std::string_view longName)
	{
		return (!shortName.empty() && shortName != "\0" && *argument == shortName) || *argument == longName;
	};

	auto next = [&c_arguments](auto& iter)
	{
		auto option = *iter;
		iter++;
		if (iter == c_arguments.end())
			throw ArgumentParserError(option + " takes an argument");
	};

	programSubmodule submodule = programSubmodule::NONE_SUBMODULE;
	bool is_help = c_arguments.empty();
	auto iter = c_arguments.begin();
	bitmode mode;

	for (; !is_help && submodule == programSubmodule::NONE_SUBMODULE && iter != c_arguments.end(); iter++)
	{
		if (*iter == "run") submodule = programSubmodule::EXECUTOR;
		else if (*iter == "convert") submodule = programSubmodule::CONVERTOR;
		else if (*iter == "help") is_help = true;
	}

	if (submodule == programSubmodule::NONE_SUBMODULE) submodule = programSubmodule::EXECUTOR;
	if (iter == c_arguments.end()) iter = c_arguments.begin();

	for (; !is_help && iter != c_arguments.end(); iter++)
	{
		if (isArgument(iter, "-h", "--help"))
			is_help = true;
		else if (isArgument(iter, "-b", "--binary"))
			mode.set(submoduleMode::BINARY);
		else if (std::filesystem::exists(*iter) && !std::filesystem::is_directory(*iter) && filename.empty())
			filename = *iter;
		else
			break;
	}

	if (is_help)
	{
		std::printf(c_help_message.data());
		std::exit(0);
	}
	else if (iter != c_arguments.end())
		throw ArgumentParserError("cannot recognize command line argument: " + std::string(*iter) + ". It is either an invalid argument or a non-existent file");
	else if (filename.empty())
		throw ArgumentParserError("provide a file");
	else if (submodule == programSubmodule::CONVERTOR && mode.test(submoduleMode::BINARY))
		throw ArgumentParserError("convertion from the binary format to the text format is not supported at the moment");

	return std::make_pair(submodule, mode);
}


static void startVirtualMachine(const std::filesystem::path& c_filename, const std::pair<programSubmodule, bitmode>& c_mode)
{
	Parser parser;
	VM vm;

	parser.parseFromFile(c_filename, c_mode.second.test(submoduleMode::BINARY));

	if (c_mode.first == programSubmodule::EXECUTOR)
		vm.run(parser.getInstructions());
	else if (c_mode.first == programSubmodule::CONVERTOR)
		parser.outputInstructions(c_filename);
	else
		throw ArgumentParserError("invalid program submodule");
}


int main(int argc, char* argv[])
{
	const int start_from = static_cast<int>(argc > 0);
	const auto c_args = std::span(argv + start_from, argc - start_from) | std::views::transform([](const char* str) { return std::string_view{str}; });
	try {
		const auto c_mode = parseArguments(c_args);
		startVirtualMachine(filename, c_mode);
	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}  