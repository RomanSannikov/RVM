#pragma once

#include <iostream> // Fix: it's temporary
#include <vector>
#include <unordered_map>

#include "instruction.hpp"

class VM
{
private:
	const unsigned c_SIZE_OF_STACK;

	std::vector<uint8_t> stack;

	// Todo: use std::move()
	std::vector<uint8_t> instructions;
	std::unordered_map<std::string, uint8_t> symbolTable;

	uint16_t programPointer;
	uint8_t stackPointer;

public:
	VM() : programPointer(0), c_SIZE_OF_STACK(15), stackPointer(0)
	{ stack.reserve(c_SIZE_OF_STACK); }

public:
	void run(std::vector<uint8_t>&);

private:
	void doInstruction(const TokenState&);
	
	void increaseStack();
	void decreaseStack();
};

