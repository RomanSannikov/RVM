#pragma once

#include <cmath>
#include <vector>
#include <cassert>
#include <memory>
#include <functional>

#include "STWGC.hpp"
#include "instruction.hpp"
#include "logging.hpp"
#include "object.hpp"
#include "StackFrameManager.hpp"
#include "utils.hpp"

class VM
{
private:
	const size_t c_STACK_SIZE = 8;
	const size_t c_POOL_SIZE = 1024;

	std::shared_ptr<std::byte[]> pool = std::make_shared<std::byte[]>(c_POOL_SIZE);
	stackType poolPointer = 0;

	std::shared_ptr<BaseGC> gc = std::make_shared<STWGC>();

	std::vector<ObjectType> objectRepresentationTable = { ObjectType::INT, ObjectType::DOUBLE, ObjectType::REF };
	std::vector<stackType> stack;
	StackFrameManager stackFrame;
	std::vector<instructionType> instructions;

	int32_t stackPointer = 0;

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
	VM()
	{
		stack.reserve(c_STACK_SIZE);
		stackFrame.pushFrame();
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

	void ld(const uint16_t&);
	void sv(const uint16_t&);

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

	void dup(const stackType&);

	void pushn(const stackType&);
	void pushs(const std::string&);
	void popn();
	void pops();

	void allocate(const stackType&);

	friend class TestFunctions;
};

