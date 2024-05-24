#include <string_view>
#include <filesystem>
#include <optional>

#include "parser.hpp"
#include "VM.hpp"
#include "EpsilonGC.hpp"
#include "STWGC.hpp"

namespace startupUtils
{

using namespace std::literals;

struct programOptions
{
	enum class programSubmodule
	{
		NONE = 0,
		EXECUTOR,
		CONVERTOR
	};

	enum class submoduleMode
	{
		NONE = 0,
		BINARY
	};

	enum class helpModes
	{
		NONE = 0,
		BYTECODE,
		GENERAL
	};

	enum class gcType
	{
		EPSILON = 0,
		STW
	};

	programSubmodule submodule = programSubmodule::NONE;
	submoduleMode submoduleMode = submoduleMode::NONE;
	gcType gcType = gcType::STW;
	std::filesystem::path filename;
};


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


static struct programOptions parseArguments(const auto& c_arguments)
{
	auto isArgument = [](const auto& argument, std::optional<std::string_view> shortName, std::optional<std::string_view> longName)
	{
		return (shortName.has_value() && *argument == shortName.value()) || (longName.has_value() && *argument == longName);
	};

	// Descr: flag either has one of the given options or has non of them
	auto optionalNext = [&c_arguments](const auto& iter, std::vector<std::pair<std::string_view, const std::function<void()>>> c_options)
	{
		auto defaultCall = [&c_options]() { if (!c_options.empty() && c_options.back().first.empty()) c_options.back().second(); };
		if (iter == c_arguments.end()) { defaultCall(); return; }
		for (const auto& option : c_options)
		{
			if (*iter == option.first)
			{
				option.second();
				return;
			}
		}
		// Descr: Last chance, default option
		if (c_options.back().first.empty()) { defaultCall(); return; }
		throw ArgumentParserError("Invalid argument");
	};

	// Descr: mandatory flag options
	auto mandatoryNext = [&c_arguments](const auto& iter, std::vector<std::pair<std::string_view, const std::function<void()>>> c_options)
	{
		if (iter == c_arguments.end()) throw ArgumentParserError("Invalid argument");
		for (const auto& option : c_options)
		{
			if (*iter == option.first)
			{
				option.second();
				return;
			}
		}
		throw ArgumentParserError("Invalid argument");
	};

	programOptions::helpModes helpMode = (c_arguments.empty() ? programOptions::helpModes::GENERAL : programOptions::helpModes::NONE);
	programOptions programOptions;
	auto iter = c_arguments.begin();

	const static std::vector<std::pair<std::string_view, const std::function<void()>>> submoduleOptions =
	{
		{"run"sv, [&programOptions]() { programOptions.submodule = programOptions::programSubmodule::EXECUTOR; }},
		{"convert"sv, [&programOptions]() { programOptions.submodule = programOptions::programSubmodule::CONVERTOR; }},
		{"help"sv, [&helpMode]() { helpMode = programOptions::helpModes::GENERAL; }},
		{{}, [&programOptions]() { programOptions.submodule = programOptions::programSubmodule::EXECUTOR; }} // Descr: Default value
	};

	const static std::vector<std::pair<std::string_view, const std::function<void()>>> helpOptions =
	{
		{"bytecode"sv, [&helpMode]() { helpMode = programOptions::helpModes::BYTECODE; }},
		{{}, [&helpMode]() { helpMode = programOptions::helpModes::GENERAL; }} // Descr: Default value
	};

	const static std::vector<std::pair<std::string_view, const std::function<void()>>> gcOptions =
	{
		{"epsilon"sv, [&programOptions]() { programOptions.gcType = programOptions::gcType::EPSILON; }},
		{"stw"sv, [&programOptions]() { programOptions.gcType = programOptions::gcType::STW; }}
	};

	// Descr: Submodule is always first
	optionalNext(iter, submoduleOptions);

	for (; (helpMode == programOptions::helpModes::NONE) && (iter != c_arguments.end()); iter++)
	{
		if (isArgument(iter, {}, "--help"sv))
			optionalNext(++iter, helpOptions);
		else if (isArgument(iter, "-h"sv, {}))
			helpMode = programOptions::helpModes::GENERAL;
		else if (isArgument(iter, "-b"sv, "--binary"sv))
			programOptions.submoduleMode = programOptions::submoduleMode::BINARY;
		else if (isArgument(iter, {}, "--gc"sv))
			mandatoryNext(++iter, gcOptions);
		else if (std::filesystem::exists(*iter) && !std::filesystem::is_directory(*iter) && programOptions.filename.empty())
			programOptions.filename = *iter;
		else
			break;
	}

	if (helpMode != programOptions::helpModes::NONE)
	{
		std::printf(helpMode == programOptions::helpModes::GENERAL ? c_help_message.data() : c_bytecode_help_message.data());
		std::exit(0);
	}
	else if (iter != c_arguments.end())
		throw ArgumentParserError("cannot recognize command line argument: " + std::string(*iter) + ". It is either an invalid argument or a non-existent file");
	else if (programOptions.filename.empty())
		throw ArgumentParserError("provide a file");
	else if (programOptions.submodule == programOptions::programSubmodule::CONVERTOR && programOptions.submoduleMode == programOptions::submoduleMode::BINARY)
		throw ArgumentParserError("convertion from the binary format to the text format is not supported at the moment");

	return programOptions;
}


static void startVirtualMachine(const programOptions& c_programOptions)
{
	std::shared_ptr<BaseGC> gc;
	if (c_programOptions.gcType == programOptions::gcType::STW)
		gc = std::make_shared<STWGC>();
	else
		gc = std::make_shared<EpsilonGC>();

	Parser parser;
	VM vm(gc);

	parser.parseFromFile(c_programOptions.filename, c_programOptions.submoduleMode == programOptions::submoduleMode::BINARY);

	if (c_programOptions.submodule == programOptions::programSubmodule::EXECUTOR)
		vm.run(parser.getInstructions());
	else if (c_programOptions.submodule == programOptions::programSubmodule::CONVERTOR)
		parser.outputInstructions(c_programOptions.filename);
	else
		throw ArgumentParserError("invalid program submodule");
}

}
