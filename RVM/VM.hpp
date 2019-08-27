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
	std::unordered_map<std::string, std::vector<uint8_t>> symbolTable;
	std::vector<uint8_t> instructions;

	uint16_t programPointer;
	uint8_t stackPointer;

	const std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 4> arithmeticFunctions =
	{ std::bind(&VM::add, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::sub, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::mul, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::divide, this, std::placeholders::_1, std::placeholders::_2) };

	/*
	std::array < std::function<void(const uint8_t&, uint8_t&)>, 3> jumpFunctions =
	{ jmp, jne,
	  je };
	*/

	const std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 3> comparisonFunctions =
	{ std::bind(&VM::eq, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::gr, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::ls, this, std::placeholders::_1, std::placeholders::_2) };

	const std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 4> binaryArithmeticFunctions =
	{ std::bind(&VM::op_and, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_or, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::op_nand, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_xor, this, std::placeholders::_1, std::placeholders::_2) };

public:
	
	VM() : c_SIZE_OF_STACK(15), stackPointer(0), programPointer(0)
	{
		stack.reserve(c_SIZE_OF_STACK);
	}

public:
	void run(const std::vector<uint8_t>&);

	void printStack() 
	{
		std::cout << std::endl << "STACK: " << std::endl; for (auto i : stack) std::cout << std::bitset<8>(i) << std::endl;
	}

private:
	void doInstruction(const TokenState&);
	
	std::string decodeString();
	
	/*
	void increaseStack();
	void decreaseStack();
	*/

	// Todo: const
	uint8_t add(const uint8_t&, const uint8_t&);
	uint8_t sub(const uint8_t&, const uint8_t&);
	uint8_t mul(const uint8_t&, const uint8_t&);
	uint8_t divide(const uint8_t&, const uint8_t&);

	// Todo: loading and saving
	const std::vector<uint8_t>& ld(const std::string&);
	void sv(const std::string&);

	void jmp(const uint8_t&, uint8_t&);
	void jne(const uint8_t&, uint8_t&);
	void je(const uint8_t&, uint8_t&);

	uint8_t eq(const uint8_t&, const uint8_t&);
	uint8_t gr(const uint8_t&, const uint8_t&);
	uint8_t ls(const uint8_t&, const uint8_t&);

	uint8_t op_and(const uint8_t&, const uint8_t&);
	uint8_t op_or(const uint8_t&, const uint8_t&);
	uint8_t op_nand(const uint8_t&, const uint8_t&);
	uint8_t op_xor(const uint8_t&, const uint8_t&);
	uint8_t op_not(const uint8_t&);

	void pushn(const uint8_t&);
	void pushs(const std::string&);
	void popn();
	void pops();

	void allocate(const std::string&, const uint8_t&);
	void del(const std::string&);
};

