#include <vector>

#include <gtest/gtest.h>

#include "testFunctions.hpp"

TEST(SimpleInstructions, Add)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_add),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), 4);
}

TEST(SimpleInstructions, Sub)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 4,
										 static_cast<instructionType>(TokenState::op_sub),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), -2);
}

TEST(SimpleInstructions, Div)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 32,
										 static_cast<instructionType>(TokenState::op_pushn), 4,
										 static_cast<instructionType>(TokenState::op_div),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), 8);
}

TEST(SimpleInstructions, Mul)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 63,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_mul),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), 126);
}

TEST(SimpleInstructions, Inc)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 63,
										 static_cast<instructionType>(TokenState::op_inc),
										 static_cast<instructionType>(TokenState::op_inc),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), 65);
}

TEST(SimpleInstructions, Dec)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 0,
										 static_cast<instructionType>(TokenState::op_dec),
										 static_cast<instructionType>(TokenState::op_dec),
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).back(), -2);
}

TEST(SimpleInstructions, DISABLED_New)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_new), 5, 'a', 'b', 'c', '\0',
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_TRUE(TestFunctions::getStack(vm).empty());
	EXPECT_TRUE(TestFunctions::getSymbolTable(vm).contains("abc"));
	EXPECT_EQ(TestFunctions::getSymbolTable(vm).at("abc").size(), 5);
}

TEST(SimpleInstructions, DISABLED_NewDel)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_new), 5, 'a', 'b', 'c', '\0',
										 static_cast<instructionType>(TokenState::op_del), 'a', 'b', 'c', '\0',
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_TRUE(TestFunctions::getStack(vm).empty());
	EXPECT_FALSE(TestFunctions::getSymbolTable(vm).contains("abc"));
}

TEST(SimpleInstructions, DISABLED_NewSv)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_pushn), 4,
										 static_cast<instructionType>(TokenState::op_new), 2, 'a', '\0',
										 static_cast<instructionType>(TokenState::op_sv), 'a', '\0',
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_TRUE(TestFunctions::getStack(vm).empty());
	EXPECT_TRUE(TestFunctions::getSymbolTable(vm).contains("a"));
	EXPECT_EQ(TestFunctions::getSymbolTable(vm).at("a").size(), 2);
	EXPECT_EQ(TestFunctions::getSymbolTable(vm).at("a")[0], 4);
	EXPECT_EQ(TestFunctions::getSymbolTable(vm).at("a")[1], 2);
}

TEST(SimpleInstructions, DISABLED_NewSvLd)
{
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 8,
										 static_cast<instructionType>(TokenState::op_pushn), 6,
										 static_cast<instructionType>(TokenState::op_pushn), 11,
										 static_cast<instructionType>(TokenState::op_new), 3, 'b', '\0',
										 static_cast<instructionType>(TokenState::op_sv), 'b', '\0',
										 static_cast<instructionType>(TokenState::op_ld), 'b', '\0',
										 static_cast<instructionType>(TokenState::op_hlt)};
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 3);
	EXPECT_TRUE(TestFunctions::getSymbolTable(vm).contains("b"));
	EXPECT_EQ(TestFunctions::getSymbolTable(vm).at("b").size(), 3);
	EXPECT_EQ(TestFunctions::getStack(vm)[0], 11);
	EXPECT_EQ(TestFunctions::getStack(vm)[1], 6);
	EXPECT_EQ(TestFunctions::getStack(vm)[2], 8);
};

TEST(SimpleInstructions, Jmp) {
	VM vm;
	std::vector<instructionType> instructions = { static_cast<instructionType>(TokenState::op_pushn), 6,
										 static_cast<instructionType>(TokenState::op_pushn), 2,
										 static_cast<instructionType>(TokenState::op_jmp), 0, 8,
										 static_cast<instructionType>(TokenState::op_hlt),
										 static_cast<instructionType>(TokenState::op_div),
										 static_cast<instructionType>(TokenState::op_hlt) };
	vm.run(instructions);
	EXPECT_EQ(TestFunctions::getStack(vm).size(), 1);
	EXPECT_EQ(TestFunctions::getStack(vm).front(), 3);
}

TEST(SimpleInstructions, Jz) {
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

TEST(SimpleInstructions, DISABLED_Call442Div)
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
	EXPECT_EQ(TestFunctions::getStack(vm).back(), 22);
}

TEST(SimpleInstructions, DISABLED_Fibonacci) {
	const std::string c_filename = "tests/data/fibonacci.txt";
	std::string lineFromScanner;

	Scanner scanner;
	Tokenizer tokenizer;
	Parser parser;

	{
		scanner.open(c_filename);

		while (!scanner.isEOF())
		{
			scanner.getLine(lineFromScanner);

			if (lineFromScanner.empty())
				continue;

			tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());
			parser.parse(tokenizer.tokens);
		}

		parser.completeParsing();
	}

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

TEST(SimpleInstructions, DISABLED_FractorialTxt) {
	const std::string c_filename = "tests/data/fractorial.txt";
	std::string lineFromScanner;

	Scanner scanner;
	Tokenizer tokenizer;
	Parser parser;

	auto fractorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	{
		scanner.open(c_filename);

		while (!scanner.isEOF())
		{
			scanner.getLine(lineFromScanner);

			if (lineFromScanner.empty())
				continue;

			tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());
			parser.parse(tokenizer.tokens);
		}

		parser.completeParsing();
	}

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

TEST(SimpleInstructions, DISABLED_FractorialBytecode) {
	const std::string c_filename = "tests/data/fractorial.rbc";
	std::string lineFromScanner;

	Scanner scanner;
	Parser parser;

	auto fractorial = [](int n, auto&& self) -> int { return (n <= 1 ? 1 : n * self(n - 1, self)); };

	{
		scanner.open(c_filename);
		parser.loadInstructions(scanner.readBinary());
	}

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
