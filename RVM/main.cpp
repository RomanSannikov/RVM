#include <iostream>
#include <span>
#include <ranges>
#include <string_view>

#include "startupUtils.hpp"


int main(int argc, char* argv[])
{
	const int start_from = static_cast<int>(argc > 0);
	const auto c_args = std::span(argv + start_from, argc - start_from) | std::views::transform([](const char* str) { return std::string_view{str}; });
	try {
		const auto programOptions = startupUtils::parseArguments(c_args);
		startupUtils::startVirtualMachine(programOptions);
	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
