#include "VM.hpp"


stackType VM::add(const stackType& a, const stackType& b) { --stackPointer; return b + a;}
stackType VM::sub(const stackType& a, const stackType& b) { --stackPointer; return b - a; }
stackType VM::mul(const stackType& a, const stackType& b) { --stackPointer; return b * a; }
stackType VM::divide(const stackType& a, const stackType& b) { --stackPointer; return b / a; }

void VM::inc() { ++stack[stackPointer - 1]; }

void VM::dec() { --stack[stackPointer - 1]; }

void VM::ld(const stackType& offset)
{
	auto objectType = reinterpret_cast<BaseObject*>(static_cast<char*>(pool.get()) + offset)->getType();
	auto getValue = [&]<Arithmetic T>() -> auto { return reinterpret_cast<Object<T>*>(static_cast<char*>(pool.get()) + offset)->getValue(); };

	switch(objectType) {
		case ObjectType::INT:
		case ObjectType::REF:
		{
			auto value = getValue.template operator()<stackType>();
			stack.push_back(value);
		} break;
		case ObjectType::DOUBLE:
		{
			// Descr: Here, I try to fit double into the stack by converting to uint and cutting to pieces.
			//	It may also break here because of this convertion
			//  Bytes of the double are stored on the stack like this: upper bytes are on the top
			union { double value_double; uint64_t value_int; } convertor;
			convertor.value_double = getValue.template operator()<double>();
			for (unsigned i = 0; i < sizeof(decltype(convertor.value_int)) / sizeof(stackType); i++)
				stack.push_back(static_cast<stackType>(convertor.value_int >> i * sizeof(stackType)));
		} break;
		default: throw RVMError("Invalid object type");
	}

	++stackPointer;
}

void VM::sv(const stackType& offset)
{
	auto objectType = reinterpret_cast<BaseObject*>(static_cast<char*>(pool.get()) + offset)->getType();
	auto setValue = [&]<Arithmetic T>(const T& value) -> auto { return reinterpret_cast<Object<T>*>(static_cast<char*>(pool.get()) + offset)->setValue(value); };

	switch(objectType) {
		case ObjectType::INT:
		case ObjectType::REF:
		{
			stack.pop_back();
			--stackPointer;
			setValue(stack.back());
		} break;
		case ObjectType::DOUBLE:
		{
			// Descr: Checkout the `ld()` function's comment for more info about double handling
			union { double value_double; uint64_t value_int; } convertor;
			convertor.value_int = 0;
			for (unsigned i = 0; i < sizeof(decltype(convertor.value_int)) / sizeof(stackType); i++) {
				convertor.value_int = (convertor.value_int << static_cast<int>(i != 0) * sizeof(stackType)) | static_cast<decltype(convertor.value_int)>(stack.back());
				stack.pop_back();
				--stackPointer;
			}
			setValue(convertor.value_double);
		} break;
		default: throw RVMError("Invalid object type");
	}

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

void VM::dup(const stackType& numberOfDuplicates) {
	assert(stack.size() >= numberOfDuplicates);
	for (stackType i = 0; i < numberOfDuplicates; i++)
		stack.push_back(stack[stack.size() - numberOfDuplicates]);
	stackPointer += numberOfDuplicates;
}

void VM::pushn(const stackType& a) {
	++stackPointer;
	stack.push_back(a);
}

void VM::pushs(const std::string& c_data)
{
	std::copy(c_data.begin(), c_data.end(), stack.begin());
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

void VM::allocate(const stackType& objectNumber)
{
	if (objectNumber < 0 || objectNumber >= objectRepresentationTable.size()) throw RVMError("Invalid object number passed to the Object Representation Table");

	auto objectType = objectRepresentationTable[objectNumber];
	stackType allocationOffset = 0;

	auto allocateObject = [&]<Arithmetic T>()
	{
		Object<T>* object = new(static_cast<char*>(pool.get()) + poolPointer) Object<T>(objectType);
		allocationOffset = reinterpret_cast<char*>(object) - pool.get();
		poolPointer += sizeof(Object<T>);
	};

	if (objectType == ObjectType::INT || objectType == ObjectType::REF) allocateObject.template operator()<stackType>();
	else if (objectType == ObjectType::DOUBLE) allocateObject.template operator()<double>();
	else throw RVMError("Invalid object type");

	// Todo: Make this depend on arguments
	if (std::isgreater(static_cast<double>(poolPointer) / static_cast<double>(c_POOL_SIZE), 0.8)) gc->run();

	stack.push_back(allocationOffset);
	stackPointer++;
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
	
	if (stack.size() > 0) a = stack.back();
	if (stack.size() > 1) b = stack[stack.size() - 2];

	auto popTwoTimes = [&]() { stack.pop_back(); stack.pop_back(); };
	auto getFunctionIndex = [&](const TokenState&& c_tokenState) -> auto
	{
		popTwoTimes();
		return static_cast<int>(c_opcode) - static_cast<int>(c_tokenState);
	};

	if (c_opcode >= TokenState::op_add && c_opcode <= TokenState::op_div)
	{
		stack.push_back(c_arithmeticFunctions[getFunctionIndex(TokenState::op_add)](a, b));
	}
	else if (c_opcode == TokenState::op_inc || c_opcode == TokenState::op_dec)
	{
		int index = static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_inc);
		incAndDecFunctions[index]();
	}
	else if (c_opcode == TokenState::op_ld)
		ld(a);
	else if (c_opcode == TokenState::op_sv)
		sv(a);
	else if ((c_opcode >= TokenState::op_jmp && c_opcode <= TokenState::op_jnz) || c_opcode == TokenState::op_call)
	{
		++programPointers.top();
		// Desc: `(int16_t)0x00FF` is intended to set the high nibble of `(int16_t)instructions[programPointers.top() + 1]` to zero, since the variable is initially of type `uint8_t`
		int16_t destination = (int16_t)(((int16_t)((int16_t)instructions[programPointers.top()]) << 8) | (int16_t)0x00FF & (int16_t)instructions[programPointers.top() + 1]);
		int index = (c_opcode != TokenState::op_call ? static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_jmp) : 0);
		// Desc: if it's the call instruction then increment `programPointer` to skip the call arguments and land on the next instructions
		if (c_opcode == TokenState::op_call) {
			auto newProgramPointer = ++programPointers.top();
			programPointers.push(newProgramPointer);
		}
		c_jumpFunctions[index](destination - 1);
	}
	else if (c_opcode >= TokenState::op_eq && c_opcode <= TokenState::op_ls)
	{
		int index = static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_eq);
		stack.push_back(c_comparisonFunctions[index](a, b));
	}
	else if (c_opcode >= TokenState::op_and && c_opcode <= TokenState::op_xor)
	{
		stack.push_back(c_binaryArithmeticFunctions[getFunctionIndex(TokenState::op_add)](a, b));
	}
	else if (c_opcode == TokenState::op_not)
	{
		stack.pop_back();
		stack.push_back(op_not(a));
	}
	else if (c_opcode == TokenState::op_dup)
		dup(instructions[++programPointers.top()]);
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
		allocate(instructions[++programPointers.top()]);
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
