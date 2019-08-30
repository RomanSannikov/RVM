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

	std::vector<int8_t> stack;
	std::unordered_map<std::string, std::vector<int8_t>> symbolTable;
	std::vector<int8_t> instructions;

	uint16_t programPointer;
	int8_t stackPointer;

	const std::array < std::function<int8_t(const int8_t&, const int8_t&)>, 4> arithmeticFunctions =
	{ std::bind(&VM::add, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::sub, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::mul, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::divide, this, std::placeholders::_1, std::placeholders::_2) };

	const std::array <std::function<void()>, 2> incAndDecFunctions =
	{ std::bind(&VM::inc, this), std::bind(&VM::dec, this) };

	const std::array < std::function<void(const int16_t&)>, 5> jumpFunctions =
	{ std::bind(&VM::jmp, this, std::placeholders::_1), std::bind(&VM::jne, this, std::placeholders::_1),
	  std::bind(&VM::je, this, std::placeholders::_1), 
	  std::bind(&VM::jz, this, std::placeholders::_1), std::bind(&VM::jnz, this, std::placeholders::_1) };

	const std::array < std::function<int8_t(const int8_t&, const int8_t&)>, 3> comparisonFunctions =
	{ std::bind(&VM::eq, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::gr, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::ls, this, std::placeholders::_1, std::placeholders::_2) };

	const std::array < std::function<int8_t(const int8_t&, const int8_t&)>, 4> binaryArithmeticFunctions =
	{ std::bind(&VM::op_and, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_or, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::op_nand, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_xor, this, std::placeholders::_1, std::placeholders::_2) };

public:
	
	VM() : c_SIZE_OF_STACK(8), stackPointer(0), programPointer(0)
	{
		stack.reserve(c_SIZE_OF_STACK);
	}

public:
	void run(const std::vector<int8_t>&);

	void printStack() 
	{
		std::cout << std::endl << "STACK: " << std::endl; for (auto i : stack) std::cout << std::bitset<8>(i) << std::endl;
	}

private:
	void doInstruction(const TokenState&);
	
	std::string decodeString();
	
	void pushToStack(const int8_t&&);
	void pushToStack(const int8_t&);
	void popFromStack();

	int8_t add(const int8_t&, const int8_t&);
	int8_t sub(const int8_t&, const int8_t&);
	int8_t mul(const int8_t&, const int8_t&);
	int8_t divide(const int8_t&, const int8_t&);

	void inc();
	void dec();

	const std::vector<int8_t>& ld(const std::string&);
	void sv(const std::string&);

	void jmp(const int16_t&);
	void jne(const int16_t&);
	void je(const int16_t&);
	void jz(const int16_t&);
	void jnz(const int16_t&);

	int8_t eq(const int8_t&, const int8_t&);
	int8_t gr(const int8_t&, const int8_t&);
	int8_t ls(const int8_t&, const int8_t&);

	int8_t op_and(const int8_t&, const int8_t&);
	int8_t op_or(const int8_t&, const int8_t&);
	int8_t op_nand(const int8_t&, const int8_t&);
	int8_t op_xor(const int8_t&, const int8_t&);
	int8_t op_not(const int8_t&);

	void pushn(const int8_t&);
	void pushs(const std::string&);
	void popn();
	void pops();

	void allocate(const std::string&, const int8_t&);
	void del(const std::string&);
};

