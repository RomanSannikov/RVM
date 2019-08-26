#pragma once

#include <iostream> // Fix: it's temporary
#include <vector>
#include <unordered_map>
#include <bitset> // Fix: just for tests

#include "parser.hpp"
#include "instruction.hpp"

class Parser;

class VM
{
private:
	const unsigned c_SIZE_OF_STACK;

	std::vector<uint8_t> stack;
	std::unordered_map<std::string, uint8_t> symbolTable;
	std::vector<uint8_t> instructions;

	uint16_t programPointer;
	uint8_t stackPointer;

public:
	
	VM() : c_SIZE_OF_STACK(15), stackPointer(0), programPointer(0)
	{
		stack.reserve(c_SIZE_OF_STACK);
	}

public:
	void run(const std::vector<uint8_t>&);

	void printStack() 
	{
		std::cout << std::endl; for (auto i : stack) std::cout << std::bitset<8>(i) << std::endl;
	}

private:
	void doInstruction(const TokenState&);
	
	std::string decodeString();
	
	/*
	void increaseStack();
	void decreaseStack();
	*/
};

