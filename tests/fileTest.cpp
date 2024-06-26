#include <vector>
#include <filesystem>

#include <gtest/gtest.h>

#include "testFunctions.hpp"

TEST(File, SimpleTestTxt) {
	const std::filesystem::path c_filename = "tests/data/simpleTest.txt";

	Parser parser;
	VM vm;

	parser.parseFromFile(c_filename, false);

	std::vector<instructionType> instructions = parser.getInstructions();
	vm.run(instructions);

	EXPECT_EQ(TestFunctions::getStack(vm).size(), 0);
}

TEST(File, Fibonacci) {
	const std::filesystem::path c_filename = "tests/data/fibonacci.txt";

	Parser parser;

	parser.parseFromFile(c_filename, false);

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 1;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], 1);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 11;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], 13);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 33;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], 34);
	}

	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[5] = 89;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], 89);
	}
}

TEST(File, FactorialTxt) {
	const std::filesystem::path c_filename = "tests/data/factorial.txt";

	Parser parser;

	auto factorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	parser.parseFromFile(c_filename, false);

	for (int i = 0; i < 5; i++)
	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[1] = i;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], factorial(i, factorial));
	}
}

TEST(File, FactorialBytecode) {
	const std::filesystem::path c_filename = "tests/data/factorial.rbc";

	Parser parser;

	auto factorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	parser.parseFromFile(c_filename, true);

	for (int i = 0; i < 5; i++)
	{
		VM vm;
		std::vector<instructionType> instructions = parser.getInstructions();
		instructions[1] = i;
		vm.run(instructions);

		EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
		EXPECT_EQ(TestFunctions::getStack(vm)[-1], factorial(i, factorial));
	}
}
