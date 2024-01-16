#pragma once

#include <string>
#include <vector>
#include <array>
#include <functional>

enum class TokenState : int8_t
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
#define NONE 0b00000000
const std::array<uint8_t, 30> c_instructionValues
				{ NONE, NONE, NONE, NONE, // add sub mul div
				NONE, NONE, // inc dec
				0b00110000, 0b00110000, // ld sv 
				0b00110000, 0b00110000, 0b00110000, 0b00110000, 0b00110000, // jmp jne je jz jnz
				NONE, NONE, NONE, // eq gr ls
				NONE, NONE, NONE, NONE, NONE, // and or nand xor not
				0b00110000, NONE, // call ret
				0b11000000, 0b00110000, NONE, NONE, // pushn pushs popn pops
				0b11000011, 0b00110000, // new del
				NONE }; // hlt

#undef NONE