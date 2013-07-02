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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
	{"VGPR",		0x00, VGPR			},
	{"SGPR",		0x00, SGPR			},
	{"TTMP",		0x70, TTMP			},
	{"ZERO",		0x80, ZERO			},
	{"INL_POS",		0x81, INL_POS		},
	{"INL_NEG",		0xC1, INL_NEG		},
	{"LITERAL",		0xFF, LITERAL		}
};

const int isa_mapped_operand_count = sizeof(isa_mapped_operand_list) 
	/ sizeof(isa_operand_type);

isa_operand* parseOperand(char *op_str, int num_bits)
{
	isa_operand* result;
	char *end;
	int i;

	result = (isa_operand *) malloc(sizeof(isa_operand));

	// Look up simple (built-in) operand types
	for (i = 0; i < isa_simple_operand_count; ++i)
		if (strncmp(op_str, isa_simple_operand_list[i].name, 
				strlen(isa_simple_operand_list[i].name)) == 0)
			break;

	if (i < isa_simple_operand_count)
	{
		result->op_code = isa_simple_operand_list[i].op_code;
		result->op_type = isa_simple_operand_list[i];
		return result;
	}

	if (tolower(op_str[0]) == 'v')
	{
		// Parse VGPR operand
		result->value = strtol((const char*) op_str+1, &end, 10);

		if (*end)
			ERROR("parsing operand (VGPR value)");

		if (result->value < 0 || result->value > 255)
			ERROR("invalid VGPR number (%d)", result->value);

		result->op_code = VGPR_OP.op_code + result->value;
		result->op_type = VGPR_OP;

		// The 9-bit SRC0 operand can represent both SGPRs and VGPRs
		if (num_bits == 9)
			result->op_code += 256;
		
		return result; 
	}
	else if (tolower(op_str[0]) == 's')
	{
		if (op_str[1] == '[')
		{
			// Parse SGPR range
			const char *delimiter = ":]";
			char *token;

			int range_start;

			token = strtok((char *) op_str+2, delimiter);
			range_start = strtol((const char*) token, &end, 10);

			if (*end)
				ERROR("parsing operand (SGPR range start)");

			// range_end is not used for now
			// Multiplicity check is a bit confusing because it has to 
			// be handled differently both for different type of instructions
			// and different type of operands (in format SMRD)

			// token = strtok(NULL, delimiter);
			// range_end = strtol((const char*) token, &end, 10);

			if (*end)
				ERROR("parsing operand (SGPR range end)");

			result->op_code = range_start;
			result->op_type = SGPR_OP;
		}
		else
		{
			// Parse SGPR operand
			result->value = strtol((const char*) op_str+1, &end, 10);

			if (*end)
				ERROR("parsing operand (SGPR value)");

			if (result->value < 0 || result->value > 103)
				ERROR("invalid SGPR number (%d)", result->value);

			result->op_code = SGPR_OP.op_code + result->value;
			result->op_type = SGPR_OP;
		}
		return result; 
	}
	else if (tolower(op_str[0]) == 't')
	{
		// Parse TTMP operand
		result->value = strtol((const char*) op_str+1, &end, 10);

		if (*end)
			ERROR("parsing operand (TTMP value)");

		if (result->value < 0 || result->value > 11)
			ERROR("invalid TTMP number (%d)", result->value);

		result->op_code = TTMP_OP.op_code + result->value;
		result->op_type = TTMP_OP;

		return result; 
	}
	else
	{
		// Parse literal constant

		if (strncmp(op_str, "0x", 2) == 0)
		{
			result->value = strtol((const char*) op_str+2, &end, 16);
		}
		else
		{
			result->value = strtol((const char*) op_str, &end, 10);
		}

		if (*end)
			ERROR("parsing operand (literal value)");

		if (num_bits != 16 && result->value == 0)
		{

			result->op_code = ZERO_OP.op_code;
			result->op_type = ZERO_OP;
		}
		else if (num_bits != 16 && result->value > 0 && result->value <= 64)
		{
			result->op_code = INL_POS_OP.op_code + result->value - 1;
			result->op_type = INL_POS_OP;
		}
		else if (num_bits != 16 && result->value >= -16 && result->value <= -1)
		{
			result->op_code = INL_NEG_OP.op_code + (-result->value) - 1;
			result->op_type = INL_NEG_OP;
		}
		else
		{
			result->op_code = LITERAL_OP.op_code;
			result->op_type = LITERAL_OP;
		}

		return result;
	}

	// At this stage this is unreachable code

	ERROR("unsupported operand type (%s", op_str);
	return result;
}

void setLiteralOperand(isa_op_code *op_code, isa_operand *operand)
{
	if (op_code->literal_set)
		ERROR("at most one literal constant can be used");
	
	op_code->literal_set = 1;
	op_code->literal = operand->value;
}