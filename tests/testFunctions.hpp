#include <vector>

#include "../RVM/scanner.hpp"
#include "../RVM/tokenizer.hpp"
#include "../RVM/parser.hpp"
#include "../RVM/VM.hpp"
#include "../RVM/instruction.hpp"

class TestFunctions
{
public:
	static const auto& getStack(const VM& vm) { return vm.stack; }
};
