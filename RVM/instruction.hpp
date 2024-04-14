#pragma once

#include <string>
#include <vector>
#include <array>

using instructionType = uint8_t;

enum class TokenState : instructionType
{
	op_add = 0,
	op_sub,
	op_mul,
	op_div,

	op_inc,
	op_dec,

	op_ld,
	op_sv,

	op_jmp,
	op_jne,
	op_je,
	op_jz,
	op_jnz,

	op_eq,
	op_gr,
	op_ls,

	op_and,
	op_or,
	op_nand,
	op_xor,
	op_not,

	op_dup,

	op_call,
	op_ret,

	op_pushn,
	op_pushs,
	op_popn,
	op_pops,

	op_new,
	op_del,

	op_hlt,

	label,

	word,
	number
};


const std::vector<std::string> c_stringInstructions =
{
	"add", "sub", "mul", "div",
	"inc", "dec",
	"ld", "sv",
	"jmp", "jne", "je", "jz", "jnz",
	"eq", "gr", "ls",
	"and", "or", "nand", "xor", "not",
	"dup",
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


/*
NUM STR NUM STR
**  **  **  **  =  byte
1value  2value
*/
static constexpr int NONE = 0b00000000;
static constexpr int NUM1 = 0b11000000;
static constexpr int STR1 = 0b00110000;
static constexpr int NUM2 = 0b00001100;
static constexpr int STR2 = 0b00000011;
const std::array<instructionType, 31> c_instructionValues
				{ NONE, NONE, NONE, NONE, // add sub mul div
				NONE, NONE, // inc dec
				NUM1, NUM1, // ld sv 
				STR1, STR1, STR1, STR1, STR1, // jmp jne je jz jnz
				NONE, NONE, NONE, // eq gr ls
				NONE, NONE, NONE, NONE, NONE, // and or nand xor not
				NUM1, // dup
				STR1, NONE, // call ret
				NUM1, STR1, NONE, NONE, // pushn pushs popn pops
				NUM1, NONE, // new del
				NONE }; // hlt
