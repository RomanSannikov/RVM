#include <vector>

#include <gtest/gtest.h>

#include "testFunctions.hpp"

TEST(SimpleInstructions, Add)
{
    VM vm;
    std::vector<int8_t> instructions = { static_cast<int8_t>(TokenState::op_pushn), 2,
                                         static_cast<int8_t>(TokenState::op_pushn), 2,
                                         static_cast<int8_t>(TokenState::op_add),
                                         static_cast<int8_t>(TokenState::op_hlt)};
    vm.run(instructions);
    EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
    EXPECT_EQ(TestFunctions::getStack(vm).back(), 4);
}

TEST(SimpleInstructions, Sub)
{
    VM vm;
    std::vector<int8_t> instructions = { static_cast<int8_t>(TokenState::op_pushn), 2,
                                         static_cast<int8_t>(TokenState::op_pushn), 4,
                                         static_cast<int8_t>(TokenState::op_sub),
                                         static_cast<int8_t>(TokenState::op_hlt)};
    vm.run(instructions);
    EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
    EXPECT_EQ(TestFunctions::getStack(vm).back(), -2);
}

TEST(SimpleInstructions, Div)
{
    VM vm;
    std::vector<int8_t> instructions = { static_cast<int8_t>(TokenState::op_pushn), 32,
                                         static_cast<int8_t>(TokenState::op_pushn), 4,
                                         static_cast<int8_t>(TokenState::op_div),
                                         static_cast<int8_t>(TokenState::op_hlt)};
    vm.run(instructions);
    EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
    EXPECT_EQ(TestFunctions::getStack(vm).back(), 8);
}

TEST(SimpleInstructions, Mul)
{
    VM vm;
    std::vector<int8_t> instructions = { static_cast<int8_t>(TokenState::op_pushn), 63,
                                         static_cast<int8_t>(TokenState::op_pushn), 2,
                                         static_cast<int8_t>(TokenState::op_mul),
                                         static_cast<int8_t>(TokenState::op_hlt)};
    vm.run(instructions);
    EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
    EXPECT_EQ(TestFunctions::getStack(vm).back(), 126);
}