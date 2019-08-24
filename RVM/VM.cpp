#include "VM.hpp"


void VM::run(std::vector<uint8_t>& instructionsFromParser)
{
	instructions = std::move(instructionsFromParser);

	// Fix: change the name
	for (const auto& i : instructions)
	{
		doInstruction(static_cast<TokenState>(i));

		++programPointer;
	}
}


void VM::doInstruction(const TokenState& opcode)
{

}