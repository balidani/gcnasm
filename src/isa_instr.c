/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA structures
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "isa_instr.h"

const isa_instr isa_instr_list[] = 
{
	{"S_ADD_U32",        0x80000000, SOP2},
	{"S_SUB_U32",        0x80800000, SOP2},
	{"S_ADD_I32",        0x81000000, SOP2},
	{"S_SUB_I32",        0x81800000, SOP2},
	{"S_ADDC_U32",       0x82000000, SOP2},
	{"S_SUBB_U32",       0x82800000, SOP2},
	{"S_MIN_I32",        0x83000000, SOP2},
	{"S_MIN_U32",        0x83800000, SOP2},
	{"S_MAX_I32",        0x84000000, SOP2},
	{"S_MAX_U32",        0x84800000, SOP2},
	{"S_CSELECT_B32",    0x85000000, SOP2},
	{"S_CSELECT_B64",    0x85800000, SOP2},
	{"S_AND_B32",        0x87000000, SOP2},
	{"S_AND_B64",        0x87800000, SOP2},
	{"S_OR_B32",         0x88000000, SOP2},
	{"S_OR_B64",         0x88800000, SOP2},
	{"S_XOR_B32",        0x89000000, SOP2},
	{"S_XOR_B64",        0x89800000, SOP2},
	{"S_ANDN2_B32",      0x8A000000, SOP2},
	{"S_ANDN2_B64",      0x8A800000, SOP2},
	{"S_ORN2_B32",       0x8B000000, SOP2},
	{"S_ORN2_B64",       0x8B800000, SOP2},
	{"S_NAND_B32",       0x8C000000, SOP2},
	{"S_NAND_B64",       0x8C800000, SOP2},
	{"S_NOR_B32",        0x8D000000, SOP2},
	{"S_NOR_B64",        0x8D800000, SOP2},
	{"S_XNOR_B32",       0x8E000000, SOP2},
	{"S_XNOR_B64",       0x8E800000, SOP2},
	{"S_LSHL_B32",       0x8F000000, SOP2},
	{"S_LSHL_B64",       0x8F800000, SOP2},
	{"S_LSHR_B32",       0x90000000, SOP2},
	{"S_LSHR_B64",       0x90800000, SOP2},
	{"S_ASHR_I32",       0x91000000, SOP2},
	{"S_ASHR_I64",       0x91800000, SOP2},
	{"S_BFM_B32",        0x92000000, SOP2},
	{"S_BFM_B64",        0x92800000, SOP2},
	{"S_MUL_I32",        0x93000000, SOP2},
	{"S_BFE_U32",        0x93800000, SOP2},
	{"S_BFE_I32",        0x94000000, SOP2},
	{"S_BFE_U64",        0x94800000, SOP2},
	{"S_BFE_I64",        0x95000000, SOP2},
	{"S_CBRANCH_G_FORK", 0x95800000, SOP2},
	{"S_ABSDIFF_I32",    0x96000000, SOP2}
};

const int isa_instr_count = sizeof(isa_instr_list) / sizeof(isa_instr);