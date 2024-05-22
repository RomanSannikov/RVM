#include <cassert>
#include <bitset>
#include <span>
#include <ranges>
#include <string_view>
#include <filesystem>
#include <utility>
#include <optional>

#include "parser.hpp"
#include "VM.hpp"


using namespace std::literals;
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
  help           print help information

HELP SUBMODULES:
  bytecode

OPTIONS:
  -b, --binary   hint that the provided file is in the rbc format
  -h, --help     print help information

EXAMPLE OF ROLANG BYTECODE:
  pushn 2
  pushn 1
  add
  popn
  hlt

For more specific help information type one of those commands:
  RVM help bycode
  RVM --help bytecode

GitHub repository for more information: https://github.com/RomanSannikov/RVM
)";

const std::string_view c_bytecode_help_message =
R"(RVM has a set of 30 instructions.

add
  sums two top stack numbers
sub
  subtracts two top stack numbers
mul
  multiplies two top stack numbers
div
  divides two top stack numbers
inc
  increments top stack element
dec
  decrements top stack element
ld     <num>
  loads <num>'s variable onto the stack
sv     <num>
  saves <num>'s variable onto the stack
jmp    <word>
  jumps to the label <word>
jne    <word>
  jumps to the label <word> if two top stack elements are not equal
je     <word>
  jumps to the label <word> if two top stack elements are equal
jz     <word>
  jumps to the label <word> if top stack element is zero
jnz    <word>
  jumps to the label <word> if top stack element is not zero
eq
  pushes 1 onto the stack if two top stack elements are equal, otherwise - 0
gr
  pushes 1 onto the stack if top stack element is greater than the second top stack element, otherwise - 0
ls
  pushes 1 onto the stack if top stack element is less than the second top stack element, otherwise - 0
and
  pushes the result of anding two top stack elements onto the stack
or
  pushes the result of oring two top stack elements onto the stack
nand
  pushes the result of nanding two top stack elements onto the stack
xor
  pushes the result of xoring two top stack elements onto the stack
not
  pushes the result of notting two top stack elements onto the stack
dup    <num>
  pushes a duplicate of the top stack element onto the stack
call   <word>
  executes a jump to the ladel <word> with saving the current stack frame to be able to return to the same place
ret
  returns to the last stack frame position (from the last call)
pushn  <num>
  pushes the given number <num>
pushs  <word>
  pushes the given string <word> ending with '\0'
popn
  pops the top number
pops
  pops the top string terminating with '\0' (pops everything until '\0')
new    <num>
  allocates a heap object of the type <num>
hlt
  halts the execution of the program

Also, labels can be used to indicate where to jump.
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

enum helpModes
{
	NONE_MODE = 0,
	BYTECODE,
	GENERAL
};


static std::pair<programSubmodule, bitmode> parseArguments(const auto& c_arguments)
{
	auto isArgument = [](const auto& argument, std::optional<std::string_view> shortName, std::optional<std::string_view> longName)
	{
		return (shortName.has_value() && *argument == shortName.value()) || (longName.has_value() && *argument == longName);
	};

	// Descr: flag either has one of the given options or has non of them
	auto optionalNext = [&c_arguments](auto& iter, std::vector<std::pair<std::string_view, const std::function<void(int)>>> c_options)
	{
		auto defaultCall = [&c_options]() { if (!c_options.empty() && c_options.back().first.empty()) c_options.back().second(0); };
		if (++iter == c_arguments.end()) { defaultCall(); return; }
		for (auto option = c_options.cbegin(); option != c_options.cend(); option++)
		{
			if (*iter == option->first)
			{
				option->second(c_options.cbegin() - option);
				return;
			}
		}
		// Descr: Last chance, default option
		if (c_options.back().first.empty()) { defaultCall(); return; }
		throw ArgumentParserError("Invalid argument");
	};

	helpModes helpMode = (c_arguments.empty() ? helpModes::GENERAL : helpModes::NONE_MODE);
	programSubmodule submodule = programSubmodule::NONE_SUBMODULE;
	auto iter = c_arguments.begin();
	bitmode mode;

	const static std::vector<std::pair<std::string_view, const std::function<void(int)>>> helpOptions =
	{
		{"bytecode"sv, [&helpMode](int mode) { helpMode = static_cast<helpModes>(mode + 1); }}, // Descr: This cast is not really safe
		{{}, [&helpMode](int) { helpMode = helpModes::GENERAL; }} // Descr: Default value
	};

	for (; (helpMode == helpModes::NONE_MODE) && (submodule == programSubmodule::NONE_SUBMODULE) && (iter != c_arguments.end()); iter++)
	{
		if (*iter == "run") submodule = programSubmodule::EXECUTOR;
		else if (*iter == "convert") submodule = programSubmodule::CONVERTOR;
		else if (*iter == "help") optionalNext(iter, helpOptions);
	}

	if (submodule == programSubmodule::NONE_SUBMODULE) submodule = programSubmodule::EXECUTOR;
	if (iter == c_arguments.end()) iter = c_arguments.begin();

	for (; !(helpMode == helpModes::NONE_MODE) && iter != c_arguments.end(); iter++)
	{
		if (isArgument(iter, {}, "--help"))
			optionalNext(iter, helpOptions);
		else if (isArgument(iter, "-h", {}))
			helpMode = helpModes::GENERAL;
		else if (isArgument(iter, "-b", "--binary"))
			mode.set(submoduleMode::BINARY);
		else if (std::filesystem::exists(*iter) && !std::filesystem::is_directory(*iter) && filename.empty())
			filename = *iter;
		else
			break;
	}

	if (helpMode != helpModes::NONE_MODE)
	{
		std::printf(helpMode == helpModes::GENERAL ? c_help_message.data() : c_bytecode_help_message.data());
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