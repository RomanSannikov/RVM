#include "VM.hpp"


int8_t VM::add(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b + a;}
int8_t VM::sub(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b - a; }
int8_t VM::mul(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b * a; }
int8_t VM::divide(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b / a; }

void VM::inc() noexcept
{
	++stack[stackPointer - 1];
}

void VM::dec() noexcept
{
	--stack[stackPointer - 1];
}

const std::vector<int8_t>& VM::ld(const std::string& variableName) noexcept
{ 
	const auto& c_it_foundResult = symbolTable.find(variableName)->second;
	stackPointer += (int8_t)c_it_foundResult.size();
	return c_it_foundResult; 
}

void VM::sv(const std::string& c_variableName) noexcept
{
	auto& it_foundResult = symbolTable.find(c_variableName)->second;
	for (auto& i : it_foundResult) { i = stack.back(); stack.pop_back(); }
	stackPointer -= (int8_t)it_foundResult.size();
}

void VM::jmp(const int16_t& c_destination) noexcept
{
	programPointers.top() = c_destination;
}

void VM::jne(const int16_t& c_destination) noexcept
{
	if (stack[stackPointer - 1] != stack[stackPointer - 2])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::je(const int16_t& c_destination) noexcept
{
	if (stack[stackPointer - 1] == stack[stackPointer - 2])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::jz(const int16_t& c_destination) noexcept
{
	if (stack[stackPointer - 1] == 0)
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

void VM::jnz(const int16_t& c_destination) noexcept
{
	if (stack[stackPointer - 1])
		programPointers.top() = c_destination;
	else
		++programPointers.top();
}

// Todo: jl, jg

int8_t VM::eq(const int8_t& a, const int8_t& b) noexcept { ++stackPointer; return a == b; }
int8_t VM::gr(const int8_t& a, const int8_t& b) noexcept { ++stackPointer; return b > a; }
int8_t VM::ls(const int8_t& a, const int8_t& b) noexcept { ++stackPointer; return b < a; }

int8_t VM::op_and(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b ^ a; }
int8_t VM::op_or(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b | a; }
int8_t VM::op_nand(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return ~(b & a); }
int8_t VM::op_xor(const int8_t& a, const int8_t& b) noexcept { --stackPointer; return b ^ a; }
int8_t VM::op_not(const int8_t& a) noexcept { return ~a; }

void VM::pushn(const int8_t& a) noexcept { ++stackPointer; stack.push_back(a); }
void VM::pushs(const std::string& c_data) noexcept
{ 
	for (const auto& i : c_data) stack.push_back(i); 
	stack.push_back(0);
	stackPointer += (int8_t)c_data.size() + 1;
}

void VM::popn() noexcept { stack.pop_back(); --stackPointer; }

void VM::pops() noexcept
{
	for (auto&& i = stack.cend(); *i != 0; --i, --stackPointer) stack.pop_back(); 
	stack.pop_back();
	--stackPointer;
}

void VM::allocate(const std::string& c_variableName, const int8_t& c_variableSize) noexcept
{ symbolTable.insert(std::make_pair(c_variableName, std::vector<int8_t>((size_t)c_variableSize))); }

void VM::del(const std::string& c_variableName) noexcept
{ symbolTable.erase(c_variableName); }


void VM::run(const std::vector<int8_t>& c_instructions) noexcept
{
	this->instructions = std::move(c_instructions);

	for (; programPointers.top() < c_instructions.size(); ++programPointers.top())
	{
		int8_t currentInstruction = c_instructions[programPointers.top()];
		TokenState opcode = static_cast<TokenState>(currentInstruction);
		if (opcode == TokenState::op_hlt)
			break;
		doInstruction(opcode);
		assert(stackPointer >= 0);

		Logger::printStack(stack);
	}
}


void VM::doInstruction(const TokenState& c_opcode) noexcept
{
	int8_t a, b;
	int index;
	
	if (stack.size() > 0) a = stack.back();
	if (stack.size() > 1) b = stack[stack.size() - 2];

	auto popTwoTimes = [&]() { stack.pop_back(); stack.pop_back(); };
	auto routine = [&](const TokenState&& c_tokenState) 
	{
		popTwoTimes();
		index = static_cast<int>(c_opcode) - static_cast<int>(c_tokenState);
		// Desc: `programPointer` has been incremented
		--programPointers.top();
	};

	++programPointers.top();

	if (c_opcode >= TokenState::op_add && c_opcode <= TokenState::op_div)
	{
		routine(TokenState::op_add);
		stack.push_back(c_arithmeticFunctions[index](a, b));
	}
	else if (c_opcode == TokenState::op_inc || c_opcode == TokenState::op_dec)
	{
		// Desc: `programPointer` has been incremented
		--programPointers.top();
		index = static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_inc);
		incAndDecFunctions[index]();
	}
	else if (c_opcode == TokenState::op_ld)
	{
		const auto& c_variableData = ld(decodeString());
		for (const auto& i : c_variableData)
			stack.push_back(i);
	}
	else if (c_opcode == TokenState::op_sv)
		sv(decodeString());
	else if ((c_opcode >= TokenState::op_jmp && c_opcode <= TokenState::op_je) || c_opcode == TokenState::op_call)
	{
		int16_t destination = (int16_t)(((int16_t)((int16_t)instructions[programPointers.top()]) << 8) | (int16_t)instructions[programPointers.top() + 1]);
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
		pushn(instructions[programPointers.top()]);
	else if (c_opcode == TokenState::op_pushs)
		pushs(decodeString());
	else if (c_opcode == TokenState::op_popn)
	{
		// Desc: `programPointer` has been incremented
		--programPointers.top();
		popn();
	}
	else if (c_opcode == TokenState::op_pops)
	{
		// Desc: `programPointer` has been incremented
		--programPointers.top();
		pops();
	}
	else if (c_opcode == TokenState::op_new)
	{
		const int8_t c_data = instructions[programPointers.top()];
		++programPointers.top();
		allocate(decodeString(), c_data);
	}
	else if (c_opcode == TokenState::op_del)
		del(decodeString());
}


std::string VM::decodeString() noexcept
{
	int8_t character = instructions[programPointers.top()];
	std::string result;

	while (character != 0)
	{
		result.push_back(character);
		character = instructions[++programPointers.top()];
	}

	return result;
}