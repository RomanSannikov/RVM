#pragma once

#include <string>
#include <vector>
#include <array>

enum class TokenState : unsigned
{
	op_add = 0,
	op_sub,
	op_mul,
	op_div,

	op_ld,
	op_sv,

	op_eq,
	op_jmp,
	op_jne,
	op_je,
	op_gr,
	op_ls,

	op_and,
	op_or,
	op_nand,
	op_xor,
	op_not,

	op_call,
	op_ret,

	op_pushn,
	op_pushs,
	op_popn,
	op_pops,

	op_new,
	op_del,

	op_hlt,

	label, // Fix: do i need the label token?

	word,
	number
};


const std::vector<std::string> c_stringInstructions =
{
	"add", "sub", "mul", "div",
	"ld", "sv",
	"eq", "jmp", "jne", "je", "gr", "ls",
	"and", "or", "nand", "xor", "not",
	"call", "ret", 
	"pushn", "pushs", "popn", "pops",
	"new", "del", 
	"hlt"
};


struct Token
{
	TokenState tokenState;
	std::string stringValue;
	unsigned lineNumber;
};

struct executingInstructionsFunctions
{
	// Todo: const
	template <typename T> T add(const T&, const T&);
	template <typename T> T sub(const T&, const T&);
	template <typename T> T mul(const T&, const T&);
	template <typename T> T div(const T&, const T&);

	template <typename T> T ld(const std::string&);
	template <typename T> void sv(const std::string&);
	
	template <typename T> T eq(const T&, const T&);
	template <typename T> void jmp(const T&, T&);
	template <typename T> void jne(const T&, T&);
	template <typename T> void je(const T&, T&);
	template <typename T> T gr(const T&, const T&);
	template <typename T> T ls(const T&, const T&);

	// Fix: think of the names
	template <typename T> T op_and(const T&, const T&);
	template <typename T> T op_or(const T&, const T&);
	template <typename T> T op_nand(const T&, const T&);
	template <typename T> T op_xor(const T&, const T&);
	template <typename T> T op_not(const T&);

	template <typename T> void call(const T&);
	template <typename T> void ret();
	
	template <typename T> void pushn(std::vector<T>&, const T&);
	template <typename T> void pushs(std::vector<T>&, const std::string&);
	template <typename T> void popn(std::vector<T>&);
	template <typename T> void pops(std::vector<T>&);
	
	template <typename T> void allocate(const T&, const std::string&);
	template <typename T> void del(const std::string&);
};

/*
NUM STR NUM STR
**  **  **  **  =  byte
1value  2value
*/
#define NONE 0b00000000
const std::array<uint8_t, 26> c_instructionValues
				{ NONE, NONE, NONE, NONE, 
				0b00110000, 0b00110000, 
				NONE, 0b00110000, 0b00110000, 0b00110000, NONE, NONE,
				NONE, NONE, NONE, NONE, NONE,
				0b00110000, NONE, 
				0b11000000, 0b00110000, NONE, NONE,
				0b11000011, 0b00110000,
				NONE };

#undef NONE