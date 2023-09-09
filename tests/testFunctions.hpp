#include <vector>

#include "../RVM/VM.hpp"
#include "../RVM/instruction.hpp"

class TestFunctions
{
    public:
    static const std::vector<int8_t> getStack(const VM& vm) { return vm.stack; }
};