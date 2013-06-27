/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA operand structures
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "isa_operand.h"

// This won't be used for now
/*
const isa_operand isa_operand_list = 
{
	{"SGPR",		0x00, 0, SGPR,		 1},
	{"VCC_LO",		0x6A, 0, VCC_LO,	 0},
	{"VCC_HI",		0x6B, 0, VCC_HI,	 0},
	{"TBA_LO",		0x6C, 0, TBA_LO,	 0},
	{"TBA_HI",		0x6D, 0, TBA_HI,	 0},
	{"TMA_LO",		0x6E, 0, TMA_LO,	 0},
	{"TMA_HI",		0x6F, 0, TMA_HI,	 0},
	{"TTMP",		0x70, 0, TTMP,		 1},
	{"M0",			0x7C, 0, M0,		 0},
	{"EXEC_LO",		0x7E, 0, EXEC_LO,	 0},
	{"EXEC_HI",		0x7F, 0, EXEC_HI,	 0},
	{"ZERO",		0x80, 0, ZERO,		 0},
	{"INL_POS",		0x81, 0, INL_POS,	 1},
	{"INL_NEG",		0xC1, 0, INL_NEG,	 1},
	{"INL_HLF",		0xF0, 0, INL_HLF,	 0},
	{"INL_HLF_N",	0xF1, 0, INL_HLF_N,	 0},
	{"INL_ONE",		0xF2, 0, INL_ONE,	 0},
	{"INL_ONE_N",	0xF3, 0, INL_ONE_N,	 0},
	{"INL_TWO",		0xF4, 0, INL_TWO,	 0},
	{"INL_TWO_N",	0xF5, 0, INL_TWO_N,	 0},
	{"INL_FOUR",	0xF6, 0, INL_FOUR,	 0},
	{"INL_FOUR_N",	0xF7, 0, INL_FOUR_N, 0},
	{"VCCZ",		0xFB, 0, VCCZ,		 0},
	{"EXECZ",		0xFC, 0, EXECZ,		 0},
	{"SCC",			0xFD, 0, SCC,		 0},
	{"LITERAL",		0xFF, 0, LITERAL,	 0}
};
const int isa_operand_count = sizeof(isa_operand_list) / sizeof(isa_operand);
*/