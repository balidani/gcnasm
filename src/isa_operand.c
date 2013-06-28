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

// A list of built-in operands 
// This is incomplete, only a few operand types are parsed this way
// Some of the types are visible outside of the list for easier parsing

const isa_operand_type isa_simple_operand_list[] = 
{
	{"VCC_LO",		0x6A, VCC_LO		},
	{"VCC_HI",		0x6B, VCC_HI		},
	{"TBA_LO",		0x6C, TBA_LO		},
	{"TBA_HI",		0x6D, TBA_HI		},
	{"TMA_LO",		0x6E, TMA_LO		},
	{"TMA_HI",		0x6F, TMA_HI		},
	{"M0",			0x7C, M0			},
	{"EXEC_LO",		0x7E, EXEC_LO		},
	{"EXEC_HI",		0x7F, EXEC_HI		},
	{"INL_HLF",		0xF0, INL_HLF		},
	{"INL_HLF_N",	0xF1, INL_HLF_N		},
	{"INL_ONE",		0xF2, INL_ONE		},
	{"INL_ONE_N",	0xF3, INL_ONE_N		},
	{"INL_TWO",		0xF4, INL_TWO 		},
	{"INL_TWO_N",	0xF5, INL_TWO_N		},
	{"INL_FOUR",	0xF6, INL_FOUR 		},
	{"INL_FOUR_N",	0xF7, INL_FOUR_N	},
	{"VCCZ",		0xFB, VCCZ			},
	{"EXECZ",		0xFC, EXECZ			},
	{"SCC",			0xFD, SCC			}
};

const int isa_simple_operand_count = sizeof(isa_simple_operand_list) 
	/ sizeof(isa_operand_type);

const isa_operand_type isa_mapped_operand_list[] = 
{
	{"SGPR",		0x00, SGPR			},
	{"TTMP",		0x70, TTMP			},
	{"ZERO",		0x80, ZERO			},
	{"INL_POS", 	0x81, INL_POS		},
	{"INL_NEG", 	0xC1, INL_NEG		},
	{"LITERAL", 	0xFF, LITERAL		}
};

const int isa_mapped_operand_count = sizeof(isa_mapped_operand_list) 
	/ sizeof(isa_operand_type);

void setLiteralOperand(isa_op_code *op_code, isa_operand operand)
{
	if (op_code->literal_set)
		ERROR("at most one literal constant can be used");
	
	op_code->literal_set = 1;
	op_code->literal = operand.value;
}