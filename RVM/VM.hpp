#pragma once

#include <stack>
#include <vector>
#include <cassert>
#include <memory>
#include <unordered_map>

#include "parser.hpp"
#include "instruction.hpp"
#include "logging.hpp"
#include "object.hpp"

using stackType = int16_t;

class Parser;

class VM
{

private:
	const unsigned c_SIZE_OF_STACK;

	// Todo: Make a real arean class
	std::shared_ptr<Object<stackType>[]> pool = std::make_shared<Object<stackType>[]>(1024);
	// Desc: Pointer to free memory
	int16_t poolPointer = 0;

	std::vector<stackType> stack;
	std::unordered_map<std::string, std::vector<instructionType>> symbolTable;
	std::vector<instructionType> instructions;

	std::stack<uint16_t> programPointers;
	int32_t stackPointer;

	const std::array < std::function<stackType(const stackType&, const stackType&)>, 4> c_arithmeticFunctions =
	{ std::bind(&VM::add, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::sub, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::mul, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::divide, this, std::placeholders::_1, std::placeholders::_2) };

	const std::array <std::function<void()>, 2> incAndDecFunctions =
	{ std::bind(&VM::inc, this), std::bind(&VM::dec, this) };

	const std::array < std::function<void(const int16_t&)>, 5> c_jumpFunctions =
	{ std::bind(&VM::jmp, this, std::placeholders::_1), std::bind(&VM::jne, this, std::placeholders::_1),
	  std::bind(&VM::je, this, std::placeholders::_1), 
	  std::bind(&VM::jz, this, std::placeholders::_1), std::bind(&VM::jnz, this, std::placeholders::_1) };

	const std::array < std::function<stackType(const stackType&, const stackType&)>, 3> c_comparisonFunctions =
	{ std::bind(&VM::eq, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::gr, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::ls, this, std::placeholders::_1, std::placeholders::_2) };

	const std::array < std::function<stackType(const stackType&, const stackType&)>, 4> c_binaryArithmeticFunctions =
	{ std::bind(&VM::op_and, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_or, this, std::placeholders::_1, std::placeholders::_2),
	  std::bind(&VM::op_nand, this, std::placeholders::_1, std::placeholders::_2), std::bind(&VM::op_xor, this, std::placeholders::_1, std::placeholders::_2) };

public:
	
	VM() : c_SIZE_OF_STACK(8), stackPointer(0)
	{
		stack.reserve(c_SIZE_OF_STACK);
		programPointers.push(0);
	}

public:
	void run(const std::vector<instructionType>&);

private:
	void doInstruction(const TokenState&);
	
	std::string decodeString();

	stackType add(const stackType&, const stackType&);
	stackType sub(const stackType&, const stackType&);
	stackType mul(const stackType&, const stackType&);
	stackType divide(const stackType&, const stackType&);

	void inc();
	void dec();

	void ld(const stackType&);
	void sv(const stackType&, const stackType&);

	void jmp(const int16_t&);
	void jne(const int16_t&);
	void je(const int16_t&);
	void jz(const int16_t&);
	void jnz(const int16_t&);

	stackType eq(const stackType&, const stackType&);
	stackType gr(const stackType&, const stackType&);
	stackType ls(const stackType&, const stackType&);

	stackType op_and(const stackType&, const stackType&);
	stackType op_or(const stackType&, const stackType&);
	stackType op_nand(const stackType&, const stackType&);
	stackType op_xor(const stackType&, const stackType&);
	stackType op_not(const stackType&);

	void pushn(const stackType&);
	void pushs(const std::string&);
	void popn();
	void pops();

	void allocate();
	void del();

	friend class TestFunctions;
};

