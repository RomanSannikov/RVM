#include "VM.hpp"


// Todo: const
uint8_t VM::add(const uint8_t& a, const uint8_t& b) { --stackPointer; return b + a;}
uint8_t VM::sub(const uint8_t& a, const uint8_t& b) { --stackPointer; return b - a; }
uint8_t VM::mul(const uint8_t& a, const uint8_t& b) { --stackPointer; return b * a; }
uint8_t VM::divide(const uint8_t& a, const uint8_t& b) { --stackPointer; return b / a; }

// Todo: loading and saving
const std::vector<uint8_t>& VM::ld(const std::string& variableName)
{ 
	const auto& foundResult = symbolTable.find(variableName)->second;
	stackPointer += (uint8_t)foundResult.size();
	return foundResult; 
}

void VM::sv(const std::string& variableName)
{
	auto& foundResult = symbolTable.find(variableName)->second;
	for (auto& i : foundResult) { i = stack.back(); stack.pop_back(); }
	stackPointer -= (uint8_t)foundResult.size();
}

uint8_t VM::eq(const uint8_t& a, const uint8_t& b) { --stackPointer; return a == b; }
uint8_t VM::gr(const uint8_t& a, const uint8_t& b) { --stackPointer; return b > a; }
uint8_t VM::ls(const uint8_t& a, const uint8_t& b) { --stackPointer; return b < a; }

uint8_t VM::op_and(const uint8_t& a, const uint8_t& b) { --stackPointer; return b and a; }
uint8_t VM::op_or(const uint8_t& a, const uint8_t& b) { --stackPointer; return b or a; }
uint8_t VM::op_nand(const uint8_t& a, const uint8_t& b) { --stackPointer; return ~(b and a); }
uint8_t VM::op_xor(const uint8_t& a, const uint8_t& b) { --stackPointer; return b xor a; }
uint8_t VM::op_not(const uint8_t& a) { return ~a; }

void VM::pushn(const uint8_t& a) { ++stackPointer; stack.push_back(a); }
void VM::pushs(const std::string& data)
{ 
	for (const auto& i : data) stack.push_back(i); 
	stack.push_back(0);
	stackPointer += (uint8_t)data.size() + 1;
}

void VM::popn() { stack.pop_back(); --stackPointer; }

void VM::pops()
{
	for (auto&& i = stack.cend(); *i != 0; --i, --stackPointer) stack.pop_back(); 
	stack.pop_back();
	--stackPointer;
}

// Todo: allocations
void VM::allocate(const std::string& variableName, const uint8_t& variableSize)
{ symbolTable.insert(std::make_pair(variableName, std::vector<uint8_t>((size_t)variableSize))); }

void VM::del(const std::string& variableName)
{ symbolTable.erase(variableName); }


void VM::run(const std::vector<uint8_t>& instructions)
{
	this->instructions = std::move(instructions);

	for (; programPointer < instructions.size(); ++programPointer)
	{
		uint8_t currentInstruction = instructions[programPointer];
		if (static_cast<TokenState>(currentInstruction) == TokenState::op_hlt)
			break;

		doInstruction(static_cast<TokenState>(currentInstruction));
	}
}


void VM::doInstruction(const TokenState& opcode)
{
	uint8_t a, b;
	int index;
	
	if (stack.size() > 0) a = stack.back();
	if (stack.size() > 1) b = stack[stack.size() - 2];

	auto popTwoTimes = [&]() { stack.pop_back(); stack.pop_back(); };
	auto lambda = [&](const TokenState&& tokenState) 
	{
		popTwoTimes();
		index = static_cast<int>(opcode) - static_cast<int>(tokenState);
	};

	++programPointer;

	if (opcode >= TokenState::op_add && opcode <= TokenState::op_div)
	{
		lambda(TokenState::op_add);
		stack.push_back(arithmeticFunctions[index](a, b));
	}
	else if (opcode == TokenState::op_ld)
	{
		const auto& variableData = ld(decodeString());
		for (const auto& i : variableData)
			stack.push_back(i);
	}
	else if (opcode == TokenState::op_sv)
		sv(decodeString());
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
	else if (opcode == TokenState::op_pushn)
		pushn(instructions[programPointer]);
	else if (opcode == TokenState::op_pushs)
		pushs(decodeString());
	else if (opcode == TokenState::op_popn)
		popn();
	else if (opcode == TokenState::op_pops)
		pops();
	else if (opcode == TokenState::op_new)
	{
		const uint8_t data = instructions[programPointer];
		++programPointer;
		allocate(decodeString(), data);
	}
	else if (opcode == TokenState::op_del)
		del(decodeString());
}


std::string VM::decodeString()
{
	uint8_t character = instructions[programPointer];
	std::string result;

	while (character != 0)
	{
		result.push_back(character);
		character = instructions[++programPointer];
	}

	return result;
}