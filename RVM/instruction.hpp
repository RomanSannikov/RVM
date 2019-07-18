#pragma once

#include <string>
#include <vector>

enum class TokenState : int
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

	op_push,
	op_pop,

	op_new,
	op_del,

	op_hlt,

	label,

	word,
	number,
	space,
	comma,
	slash
};

struct Token
{
	TokenState tokenState;
	std::string stringValue;
};

/*
NUM STR NUM STR
**  **  **  **  =  byte
1value  2value
*/
#define NONE 0b00000000
const int8_t c_instructionValues[24] = 
				{ NONE, NONE, NONE, NONE, 
				0b00110000, 0b00110000, 
				NONE, 0b00110000, 0b00110000, 0b00110000, NONE, NONE,
				NONE, NONE, NONE, NONE, NONE,
				0b00110000, NONE, 
				0b11110000, NONE,
				0b11000011, 0b00110000,
				NONE};
#undef NONE