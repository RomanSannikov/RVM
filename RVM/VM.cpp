#include "VM.hpp"


stackType VM::add(const stackType& a, const stackType& b) { --stackPointer; return b + a;}
stackType VM::sub(const stackType& a, const stackType& b) { --stackPointer; return b - a; }
stackType VM::mul(const stackType& a, const stackType& b) { --stackPointer; return b * a; }
stackType VM::divide(const stackType& a, const stackType& b) { --stackPointer; return b / a; }

void VM::inc() { ++stack[stackPointer - 1]; }

void VM::dec() { --stack[stackPointer - 1]; }

void VM::ld(const uint16_t& variableNumber)
{
	auto offset = stackFrame.getVariable(variableNumber);
	auto objectType = reinterpret_cast<BaseObject*>(static_cast<std::byte*>(pool.get()) + offset)->getType();
	auto getValue = [&]<Arithmetic T>() -> auto { return reinterpret_cast<Object<T>*>(static_cast<std::byte*>(pool.get()) + offset)->getValue(); };

	switch(objectType) {
		case ObjectType::INT:
		case ObjectType::REF:
		{
			auto value = getValue.template operator()<stackType>();
			stack.push_back(value);
			++stackPointer;
		} break;
		case ObjectType::DOUBLE:
		{
			// Descr: Here, I try to fit double into the stack by converting to uint and cutting to pieces.
			//	It may also break here because of this convertion
			//  Bytes of the double are stored on the stack like this: upper bytes are on the top
			union { double value_double; uint64_t value_int; } convertor;
			convertor.value_double = getValue.template operator()<double>();
			for (unsigned i = 0; i < sizeof(decltype(convertor.value_int)) / sizeof(stackType); i++) {
				stack.push_back(static_cast<stackType>(convertor.value_int >> i * sizeof(stackType)));
				++stackPointer;
			}
		} break;
		default: throw ExecutorError("Invalid object type");
	}
}

void VM::sv(const uint16_t& variableNumber)
{
	auto offset = stackFrame.getVariable(variableNumber);
	auto objectType = reinterpret_cast<BaseObject*>(static_cast<std::byte*>(pool.get()) + offset)->getType();
	auto setValue = [&]<Arithmetic T>(const T& value) -> auto { return reinterpret_cast<Object<T>*>(static_cast<std::byte*>(pool.get()) + offset)->setValue(value); };

	switch(objectType) {
		case ObjectType::INT:
		case ObjectType::REF:
		{
			setValue(stack.back());
			stack.pop_back();
			--stackPointer;
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
		default: throw ExecutorError("Invalid object type");
	}
}

void VM::jmp(const int16_t& c_destination) { stackFrame.setProgramPointer(c_destination); }

void VM::jne(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] != stack[stackPointer - 2])
		stackFrame.setProgramPointer(c_destination);
	else
		stackFrame.incProgramPointer();
}

void VM::je(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] == stack[stackPointer - 2])
		stackFrame.setProgramPointer(c_destination);
	else
		stackFrame.incProgramPointer();
}

void VM::jz(const int16_t& c_destination)
{
	if (stack[stackPointer - 1] == 0)
		stackFrame.setProgramPointer(c_destination);
	else
		stackFrame.incProgramPointer();
}

void VM::jnz(const int16_t& c_destination)
{
	if (stack[stackPointer - 1])
		stackFrame.setProgramPointer(c_destination);
	else
		stackFrame.incProgramPointer();
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
	std::copy_n(stack.end() - numberOfDuplicates, numberOfDuplicates, std::back_inserter(stack));
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
	if (objectNumber < 0 || objectNumber >= objectRepresentationTable.size()) throw ExecutorError("Invalid object number passed to the Object Representation Table");

	auto objectType = objectRepresentationTable[objectNumber];
	stackType allocationOffset = 0;

	auto allocateObject = [&]<Arithmetic T>()
	{
		Object<T>* object = new(static_cast<std::byte*>(pool.get()) + poolPointer) Object<T>(objectType);
		allocationOffset = reinterpret_cast<std::byte*>(object) - pool.get();
		poolPointer += sizeof(Object<T>);
	};

	if (objectType == ObjectType::INT || objectType == ObjectType::REF) allocateObject.template operator()<stackType>();
	else if (objectType == ObjectType::DOUBLE) allocateObject.template operator()<double>();
	else throw ExecutorError("Invalid object type");

	stackFrame.addVariable(allocationOffset);

	// Todo: Make this depend on arguments
	if (std::isgreater(static_cast<double>(poolPointer) / static_cast<double>(c_POOL_SIZE), 0.8)) gc->run(stackFrame, pool, poolPointer);
}


void VM::run(const std::vector<instructionType>& c_instructions)
{
	this->instructions = std::move(c_instructions);

	for (; stackFrame.getProgramPointer() < c_instructions.size(); stackFrame.incProgramPointer())
	{
		instructionType currentInstruction = c_instructions[stackFrame.getProgramPointer()];
		TokenState opcode = static_cast<TokenState>(currentInstruction);
		if (opcode == TokenState::op_hlt)
			break;
		Logger::print("Instruction: " + Logger::getInstructionName(currentInstruction));
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
	else if (c_opcode == TokenState::op_ld) {
		stackFrame.incProgramPointer();
		ld(instructions[stackFrame.getProgramPointer()]);
	} else if (c_opcode == TokenState::op_sv) {
		stackFrame.incProgramPointer();
		sv(instructions[stackFrame.getProgramPointer()]);
	} else if ((c_opcode >= TokenState::op_jmp && c_opcode <= TokenState::op_jnz) || c_opcode == TokenState::op_call)
	{
		stackFrame.incProgramPointer();
		// Desc: `(int16_t)0x00FF` is intended to set the high nibble of `(int16_t)instructions[stackFrame.getProgramPointer() + 1]` to zero, since the variable is initially of type `uint8_t`
		int16_t destination = (int16_t)(((int16_t)((int16_t)instructions[stackFrame.getProgramPointer()]) << 8) | (int16_t)0x00FF & (int16_t)instructions[stackFrame.getProgramPointer() + 1]);
		int index = (c_opcode != TokenState::op_call ? static_cast<int>(c_opcode) - static_cast<int>(TokenState::op_jmp) : 0);
		// Desc: if it's the call instruction then increment `programPointer` to skip the call arguments and land on the next instructions
		if (c_opcode == TokenState::op_call) {
			stackFrame.incProgramPointer();
			auto newProgramPointer = stackFrame.getProgramPointer();
			stackFrame.pushFrame(newProgramPointer);
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
	else if (c_opcode == TokenState::op_dup) {
		stackFrame.incProgramPointer();
		dup(instructions[stackFrame.getProgramPointer()]);
	} else if (c_opcode == TokenState::op_ret)
		stackFrame.popFrame();
	else if (c_opcode == TokenState::op_pushn) {
		stackFrame.incProgramPointer();
		pushn(instructions[stackFrame.getProgramPointer()]);
	} else if (c_opcode == TokenState::op_pushs)
		pushs(decodeString());
	else if (c_opcode == TokenState::op_popn)
		popn();
	else if (c_opcode == TokenState::op_pops)
		pops();
	else if (c_opcode == TokenState::op_new) {
		stackFrame.incProgramPointer();
		allocate(instructions[stackFrame.getProgramPointer()]);
	}
}


std::string VM::decodeString()
{
	stackFrame.incProgramPointer();
	instructionType character = instructions[stackFrame.getProgramPointer()];
	std::string result;

	while (character != 0)
	{
		result.push_back(character);
		stackFrame.incProgramPointer();
		character = instructions[stackFrame.getProgramPointer()];
	}

	return result;
}
