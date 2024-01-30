#pragma once

#include <iostream>
#include <concepts>
#include <vector>

#include "instruction.hpp"

template<class T>
concept Integral = std::is_integral<T>::value;

class Logger {
public:
	template <Integral T>
	static void printInstructionName(const T& instruction) {
		#ifndef NDEBUG
		std::cout << getInstructionName(instruction) << std::endl;
		#endif
	}

	template <Integral T>
	static void printInstructions(const std::vector<T>& instructions)
	{
		#ifndef NDEBUG
		for (size_t i = 0; i < instructions.size(); i++)
			std::printf("%d: %s (%s)\n", i, std::bitset<sizeof(T) * 8>(instructions[i]).to_string().c_str(), getInstructionName(instructions[i]).c_str());
		#endif
	}

	template <Integral T, Integral K>
	static void printStack(const std::vector<T>& stack, const K& instruction) {
		printStack(stack, "(" + getInstructionName(instruction) + ")");
	}

	template <Integral T>
	static void printStack(const std::vector<T>& stack, const std::string message = "")
	{
		#ifndef NDEBUG
		std::cout << std::endl << "STACK: " << message << std::endl;
		for (auto i : stack) std::cout << std::bitset<sizeof(T) * 8>(i) << std::endl;
		#endif
	}

	static void print(const std::string& str)
	{
		#ifndef NDEBUG
		std::cout << str << std::endl;
		#endif
	}

	template <Integral T>
	static std::string getInstructionName(const T& instruction) { return (instruction < c_stringInstructions.size() ? c_stringInstructions[instruction] : ""); }
};
