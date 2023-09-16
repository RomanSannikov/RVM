#include <vector>

#include "../RVM/VM.hpp"
#include "../RVM/instruction.hpp"

class TestFunctions
{
public:
    static const auto& getStack(const VM& vm) { return vm.stack; }
    static const auto& getSymbolTable(const VM& vm) { return vm.symbolTable; }
};