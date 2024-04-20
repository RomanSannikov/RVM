#include <vector>

#include <gtest/gtest.h>

#include "testFunctions.hpp"

TEST(Instructions, Add)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_add),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 4);
}

TEST(Instructions, Sub)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 4,
										 static_cast<instructionType>(TokenState::op_sub),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], -2);
}

TEST(Instructions, Div)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 32,
										 static_cast<instructionType>(TokenState::op_pushn), 4,
										 static_cast<instructionType>(TokenState::op_div),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 8);
}

TEST(Instructions, Mul)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 63,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_mul),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 126);
}

TEST(Instructions, Inc)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 63,
										 static_cast<instructionType>(TokenState::op_inc),
										 static_cast<instructionType>(TokenState::op_inc),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 65);
}

TEST(Instructions, Dec)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 0,
										 static_cast<instructionType>(TokenState::op_dec),
										 static_cast<instructionType>(TokenState::op_dec),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], -2);
}

TEST(Instructions, Jmp) {
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 6,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_jmp), 0, 8,
										 static_cast<instructionType>(TokenState::op_hlt),
										 static_cast<instructionType>(TokenState::op_div),
										 static_cast<instructionType>(TokenState::op_hlt) };
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[0], 3);
}

TEST(Instructions, Jz) {
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 5,
										 static_cast<instructionType>(TokenState::op_eq),
										 static_cast<instructionType>(TokenState::op_jz), 0, 8,
										 static_cast<instructionType>(TokenState::op_popn),
										 static_cast<instructionType>(TokenState::op_popn),
										 static_cast<instructionType>(TokenState::op_popn),
										 static_cast<instructionType>(TokenState::op_hlt) };
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 0);
}

TEST(Instructions, Call442Div)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_call), 0, 4,
										 static_cast<instructionType>(TokenState::op_hlt),
										 static_cast<instructionType>(TokenState::op_pushn), 44,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_div),
										 static_cast<instructionType>(TokenState::op_ret)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 22);
}

TEST(Instructions, Dup)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 99,
										 static_cast<instructionType>(TokenState::op_dup), 1,
										 static_cast<instructionType>(TokenState::op_dup), 2};
	vm.run(instructions);
	auto& stack = TestFunctions::getStack(vm);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 4);
	for (int i = 0; i < 4; i++) EXPECT_EQ(TestFunctions::getStack(vm)[i], 99);
}

TEST(Instructions, New3Times)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_new), 0,
										 static_cast<instructionType>(TokenState::op_new), 1,
										 static_cast<instructionType>(TokenState::op_new), 2};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 0);
}

TEST(Instructions, NewSvInt)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 99,
										 static_cast<instructionType>(TokenState::op_new), 0,
										 static_cast<instructionType>(TokenState::op_sv), 0};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 0);
}

TEST(Instructions, NewSvIntLd)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 99,
										 static_cast<instructionType>(TokenState::op_new), 0,
										 static_cast<instructionType>(TokenState::op_sv), 0,
										 static_cast<instructionType>(TokenState::op_ld), 0};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm)[-1], 99);
}
