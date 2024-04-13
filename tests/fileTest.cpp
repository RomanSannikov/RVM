#include <vector>

#include <gtest/gtest.h>

#include "testFunctions.hpp"

TEST(File, SimpleTestTxt) {
	const std::string c_filename = "tests/data/simpleTest.txt";

	Parser parser;
	VM vm;

	parser.parseFromFile(c_filename, false);

	std::vector<instructionType> instructions = parser.getInstructions();
	vm.run(instructions);

	EXPECT_EQ(TestFunctions::getStack(vm).size(), 0);
}

TEST(File, DISABLED_Fibonacci) {
	const std::string c_filename = "tests/data/fibonacci.txt";

	Parser parser;

	parser.parseFromFile(c_filename, false);

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 1;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), 1);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 11;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), 13);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 33;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), 34);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 89;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), 89);
	}
}

TEST(File, DISABLED_FractorialTxt) {
	const std::string c_filename = "tests/data/fractorial.txt";

	Parser parser;

	auto fractorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	parser.parseFromFile(c_filename, false);

	for (int i = 0; i < 5; i++)
	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[1] = i;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), fractorial(i, fractorial));
	}
}

TEST(File, DISABLED_FractorialBytecode) {
	const std::string c_filename = "tests/data/fractorial.rbc";

	Parser parser;

	auto fractorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	parser.parseFromFile(c_filename, true);

	for (int i = 0; i < 5; i++)
	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[1] = i;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm).back(), fractorial(i, fractorial));
	}
}
