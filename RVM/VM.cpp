#include "VM.hpp"


// Todo: const
uint8_t add(const uint8_t& a, const uint8_t& b) { return b + a; }
uint8_t sub(const uint8_t& a, const uint8_t& b) { return b - a; }
uint8_t mul(const uint8_t& a, const uint8_t& b) { return b * a; }
uint8_t divide(const uint8_t& a, const uint8_t& b) { return b / a; }

// Todo: loading and saving
uint8_t ld(const std::string&);
void sv(const std::string&);

// Todo: jumps
/*
void jmp(const uint8_t&, uint8_t&);
void jne(const uint8_t&, uint8_t&);
void je(const uint8_t&, uint8_t&);
*/

uint8_t eq(const uint8_t& a, const uint8_t& b) { return a == b; }
uint8_t gr(const uint8_t& a, const uint8_t& b) { return b > a; }
uint8_t ls(const uint8_t& a, const uint8_t& b) { return b < a; }

uint8_t op_and(const uint8_t& a, const uint8_t& b) { return b and a; }
uint8_t op_or(const uint8_t& a, const uint8_t& b) { return b or a; }
uint8_t op_nand(const uint8_t& a, const uint8_t& b) { return ~(b and a); }
uint8_t op_xor(const uint8_t& a, const uint8_t& b) { return b xor a; }
uint8_t op_not(const uint8_t& a) { return ~a; }

/*
void pushn(std::vector& stack, const uint8_t& a) { stack.push_back(); }

void pushs(std::vector& stack, const const std::string& str)
{ for (const auto& i : str) stack.push_back(i); }

void popn(std::vector& stack) { stack.pop_back(); }

void pops(std::vector& stack)
{ for (const auto& i = stack.end(); *i != 0; --i) stack.push_back(i); }
*/

// Todo: allocations
void allocate(const uint8_t&, const std::string&);
void del(const std::string&);


std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 4> arithmeticFunctions =
{ add, sub,
  mul, divide };


/*
std::array < std::function<void(const uint8_t&, uint8_t&)>, 3> jumpFunctions =
{ jmp, jne,
  je };
*/


std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 3> comparisonFunctions =
{ eq, gr,
  ls };


std::array < std::function<uint8_t(const uint8_t&, const uint8_t&)>, 4> binaryArithmeticFunctions =
{ op_and, op_or,
  op_nand, op_xor };



void VM::run(const std::vector<uint8_t>& instructions)
{
	this->instructions = std::move(instructions);

	// Fix: change the name
	for (const auto& instruction : instructions)
	{
		if (static_cast<TokenState>(instruction) == TokenState::op_hlt)
			break;

		doInstruction(static_cast<TokenState>(instruction));
	}
}


void VM::doInstruction(const TokenState& opcode)
{
	uint8_t a = stack.back(), b = stack[stack.size() - 2];
	int index;

	try
	{
		a = stack.back();
		b = stack[stack.size() - 2];
	}
	catch (const std::exception&) {}

	auto popTwoTimes = [&]() { stack.pop_back(); stack.pop_back(); };
	auto lambda = [&](const TokenState&& tokenState) 
	{
		popTwoTimes();
		index = static_cast<int>(opcode) - static_cast<int>(tokenState);
		--stackPointer;
	};

	if (opcode >= TokenState::op_add && opcode <= TokenState::op_div)
	{
		lambda(TokenState::op_add);
		stack.push_back(arithmeticFunctions[index](a, b));
	}
	/*
	else if (opcode == TokenState::op_ld)
	{
		stack.push_back(ld(decodeString()));
		++stackPointer;
	}
	else if (opcode == TokenState::op_sv)
	{
		sv(decodeString());
	}
	*/
	else if (opcode >= TokenState::op_jmp && opcode <= TokenState::op_je)
	{
		// Todo: jump stack
		// index = static_cast<int>(TokenState::op_jmp) - static_cast<int>(opcode);
		// jumpFunctions[static_cast<int>(opcode)](a, b);
	}
	else if (opcode >= TokenState::op_eq && opcode <= TokenState::op_ls)
	{
		lambda(TokenState::op_eq);
		stack.push_back(comparisonFunctions[index](a, b));
	}
	else if (opcode >= TokenState::op_and && opcode <= TokenState::op_xor)
	{
		lambda(TokenState::op_add);
		stack.push_back(binaryArithmeticFunctions[index](a, b));
	}
	else if (opcode == TokenState::op_not)
	{
		stack.pop_back();
		stack.push_back(op_not(a));
	}
	// Todo: continue with pushn
}


std::string VM::decodeString()
{
	++programPointer;

	uint8_t character = instructions[programPointer];
	std::string result;

	while (character != 0);
	{
		result.push_back(character);
		character = instructions[programPointer];
	}

	return result;
}