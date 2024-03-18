#include "VM.hpp"


stackType VM::add(const stackType& a, const stackType& b) { --stackPointer; return b + a;}
stackType VM::sub(const stackType& a, const stackType& b) { --stackPointer; return b - a; }
stackType VM::mul(const stackType& a, const stackType& b) { --stackPointer; return b * a; }
stackType VM::divide(const stackType& a, const stackType& b) { --stackPointer; return b / a; }

void VM::inc() { ++stack[stackPointer - 1]; }

void VM::dec() { --stack[stackPointer - 1]; }

void VM::ld(const stackType& a)
{
	stack.push_back(pool.get()[a].get());
	++stackPointer;
}

void VM::sv(const stackType& a, const stackType& b)
{
	pool.get()[b].set(a);
	stack.pop_back();
	--stackPointer;
}

void VM::jmp(const int16_t& c_destination) { programPointers.top() = c_destination; }

void VM::jne(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] != stack[stackPointer - 2])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::je(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] == stack[stackPointer - 2])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::jz(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] == 0)
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::jnz(const int16_t& c_destination)
{
	if (stack[stackPointer - 1])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

// Todo: jl, jg

stackType VM::eq(const stackType& a, const stackType& b) { ++stackPointer; return a == b; }
stackType VM::gr(const stackType& a, const stackType& b) { ++stackPointer; return b > a; }
stackType VM::ls(const stackType& a, const stackType& b) { ++stackPointer; return b < a; }

stackType VM::op_and(const stackType& a, const stackType& b) { --stackPointer; return b ^ a; }
stackType VM::op_or(const stackType& a, const stackType& b) { --stackPointer; return b | a; }
stackType VM::op_nand(const stackType& a, const stackType& b) { --stackPointer; return ~(b & a); }
stackType VM::op_xor(const stackType& a, const stackType& b) { --stackPointer; return b ^ a; }
stackType VM::op_not(const stackType& a) { return ~a; }

void VM::pushn(const stackType& a) {
	++stackPointer;
	stack.push_back(a);
}

void VM::pushs(const std::string& c_data)
{ 
	for (const auto& i : c_data) stack.push_back(i); 
	stack.push_back(0);
	stackPointer += (stackType)c_data.size() + 1;
}

void VM::popn() {
	stack.pop_back();
	--stackPointer;
}

void VM::pops()
{
	for (auto&& i = stack.cend(); *i != 0; --i, --stackPointer) stack.pop_back(); 
	stack.pop_back();
	--stackPointer;
}

void VM::allocate()
{
	// Todo: Allocate different amount of bytes
	// Desc: Allocation of "an object". But, actually, this just the same variable that is stored on the stack
	auto allocationOffset = (new(pool.get() + poolPointer) Object<stackType>) - pool.get();
	stack.push_back(allocationOffset);
	stackPointer++;
	poolPointer += sizeof(Object<stackType>);
}

void VM::del()
{
	// Todo: Consider this either removing or implementing
}


void VM::run(const std::vector<instructionType>& c_instructions)
{
	this->instructions = std::move(c_instructions);

	for (; programPointers.top() < c_instructions.size(); ++programPointers.top())
	{
		instructionType currentInstruction = c_instructions[programPointers.top()];
		TokenState opcode = static_cast<TokenState>(currentInstruction);
		if (opcode == TokenState::op_hlt)
			break;
		doInstruction(opcode);
		assert(stackPointer >= 0);

		Logger::printStack(stack, currentInstruction);
	}
}


void VM::doInstruction(const TokenState& c_opcode)
{
	stackType a, b;
	int index;
	
	if (stack.size() > 0) a = stack.back();
	if (stack.size() > 1) b = stack[stack.size() - 2];

	auto popTwoTimes = [&]() { stack.pop_back(); stack.pop_back(); };
	auto routine = [&](const TokenState&& c_tokenState) 
	{
		popTwoTimes();
		index = static_cast<int>(c_opcode) - static_cast<int>(c_tokenState);
	};

	if (c_opcode >= TokenState::op_add && c_opcode <= TokenState::op_div)
	{
		routine(TokenState::op_add);
		stack.push_back(c_arithmeticFunctions[index](a, b));
	}
	else if (c_opcode == TokenState::op_inc || c_opcode == TokenState::op_dec)
	{
		index = static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_inc);
		incAndDecFunctions[index]();
	}
	else if (c_opcode == TokenState::op_ld)
		ld(a);
	else if (c_opcode == TokenState::op_sv)
		sv(a, b);
	else if ((c_opcode >= TokenState::op_jmp && c_opcode <= TokenState::op_jnz) || c_opcode == TokenState::op_call)
	{
		++programPointers.top();
		// Desc: `(int16_t)0x00FF` is intended to set the high nibble of `(int16_t)instructions[programPointers.top() + 1]` to zero, since the variable is initially of type `uint8_t`
		int16_t destination = (int16_t)(((int16_t)((int16_t)instructions[programPointers.top()]) << 8) | (int16_t)0x00FF & (int16_t)instructions[programPointers.top() + 1]);
		index = (c_opcode != TokenState::op_call ? static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_jmp) : 0);
		// Desc: if it's the call instruction then increment `programPointer` to skip the call arguments and land on the next instructions
		if (c_opcode == TokenState::op_call) {
			auto newProgramPointer = ++programPointers.top();
			programPointers.push(newProgramPointer);
		}
		c_jumpFunctions[index](destination - 1);
	}
	else if (c_opcode >= TokenState::op_eq && c_opcode <= TokenState::op_ls)
	{
		index = static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_eq);
		stack.push_back(c_comparisonFunctions[index](a, b));
	}
	else if (c_opcode >= TokenState::op_and && c_opcode <= TokenState::op_xor)
	{
		routine(TokenState::op_add);
		stack.push_back(c_binaryArithmeticFunctions[index](a, b));
	}
	else if (c_opcode == TokenState::op_not)
	{
		stack.pop_back();
		stack.push_back(op_not(a));
	}
	else if (c_opcode == TokenState::op_ret)
		programPointers.pop();
	else if (c_opcode == TokenState::op_pushn)
		pushn(instructions[++programPointers.top()]);
	else if (c_opcode == TokenState::op_pushs)
		pushs(decodeString());
	else if (c_opcode == TokenState::op_popn)
		popn();
	else if (c_opcode == TokenState::op_pops)
		pops();
	else if (c_opcode == TokenState::op_new)
		allocate();
	else if (c_opcode == TokenState::op_del)
		del();
}


std::string VM::decodeString()
{
	instructionType character = instructions[++programPointers.top()];
	std::string result;

	while (character != 0)
	{
		result.push_back(character);
		character = instructions[++programPointers.top()];
	}

	return result;
}
