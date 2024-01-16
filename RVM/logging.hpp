#pragma once

#include <iostream>
#include <concepts>
#include <vector>

#include "instruction.hpp"

template<class T>
concept Integral = std::is_integral<T>::value;

class Logger {
public:
	static void printInstructionName(const Integral auto& instruction) {
		#ifndef NDEBUG
		std::cout << getInstructionName(instruction) << std::endl;
		#endif
	}

	static void printInstructions(const std::vector<Integral auto>& instructions)
	{
		#ifndef NDEBUG
		for (size_t i = 0; i < instructions.size(); i++)
			std::printf("%d: %s (%s)\n", i, std::bitset<8>(instructions[i]).to_string().c_str(), getInstructionName(instructions[i]).c_str());
		#endif
	}

	static void printStack(const std::vector<Integral auto>& stack)
	{
		#ifndef NDEBUG
		std::cout << std::endl << "STACK: " << std::endl;
		for (auto i : stack) std::cout << std::bitset<8>(i) << std::endl;
		#endif
	}

	static void print(const std::string& str)
	{
		#ifndef NDEBUG
		std::cout << str << std::endl;
		#endif
	}

private:
	static std::string getInstructionName(const Integral auto& instruction) { return (instruction < c_stringInstructions.size() ? c_stringInstructions[instruction] : ""); }
};
