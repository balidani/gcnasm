/*
 * AMD GCN ISA Assembler
 *
 * VOP2 instruction parser
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

#include "vop2.h"

/**
 * Parses instructions with a VOP2 encoding
 * 
 * MAGIC (1) | OP (6) | VDST (8) | VSRC1 (8) | SSRC0 (8) | [LITERAL (32)]
 */
void parseVOP2Instruction(isa_instr instr, char **args)
{
	char *vdst_str, *src0_str, *vsrc1_str; 	// And *vcc_str

	isa_operand vdst_op, vsrc1_op, src0_op;	// ISA operand structs
	isa_op_code op_code;					// Generated opcode struct

	// Setup arguments
	vdst_str	= args[0];
	//vcc_str	= args[1]; // Not used yet
	src0_str	= args[1];
	vsrc1_str	= args[2];

	// Parse operands
	op_code.code = instr.op_code;
	op_code.literal_set = 0;

	// VDST
	vdst_op = parseVOP2Operand(vdst_str, 0);

	if (vdst_op.op_type.type != VGPR)
		ERROR("VDST must be of VGPR type");

	op_code.code |= vdst_op.op_code << 17;

	// SRC0
	src0_op = parseVOP2Operand(src0_str, 1);

	if (src0_op.op_type.type == LITERAL)
		setLiteralOperand(&op_code, src0_op);

	op_code.code |= src0_op.op_code;	

	// VSRC1
	vsrc1_op = parseVOP2Operand(vsrc1_str, 0);

	if (vsrc1_op.op_type.type != VGPR)
		ERROR("VSRC1 must be of VGPR type");

	op_code.code |= vsrc1_op.op_code << 9;

	printf("0x%08x", op_code.code);

	if (op_code.literal_set)
	{
		printf(" 0x%08x\n", op_code.literal);
	}
	else
	{
		printf("\n");
	}
}

/**
 * Parses VOP type operands
 *
 * TODO:
 * (Actually, there are no VOP type operands.
 * Operand parsing will have to be moved.
 * For now I'll stick to ugly code reuse.)
 */
isa_operand parseVOP2Operand(char *op_str, int is_src0)
{
	isa_operand result;
	char *end;
	int i;

	// Look up simple (built-in) operand types
	for (i = 0; i < isa_simple_operand_count; ++i)
		if (strncmp(op_str, isa_simple_operand_list[i].name, 
				strlen(isa_simple_operand_list[i].name)) == 0)
			break;

	if (i < isa_simple_operand_count)
	{
		result.op_code = isa_simple_operand_list[i].op_code;
		result.op_type = isa_simple_operand_list[i];
		return result;
	}

	if (tolower(op_str[0]) == 'v')
	{
		// Parse VGPR operand
		result.value = strtol((const char*) op_str+1, &end, 10);

		if (*end)
			ERROR("parsing operand (VGPR value)");

		if (result.value < 0 || result.value > 255)
			ERROR("invalid VGPR number (%d)", result.value);

		result.op_code = VGPR_OP.op_code + result.value;
		result.op_type = VGPR_OP;

		// The 9-bit SRC0 operand can represent both SGPRs and VGPRs
		if (is_src0)
			result.op_code += 256;
		
		return result; 
	}
	else if (tolower(op_str[0]) == 's')
	{
		// Parse SGPR operand
		result.value = strtol((const char*) op_str+1, &end, 10);

		if (*end)
			ERROR("parsing operand (SGPR value)");

		if (result.value < 0 || result.value > 103)
			ERROR("invalid SGPR number (%d)", result.value);

		result.op_code = SGPR_OP.op_code + result.value;
		result.op_type = SGPR_OP;

		return result; 
	}
	else if (tolower(op_str[0]) == 't')
	{
		// Parse TTMP operand
		result.value = strtol((const char*) op_str+1, &end, 10);

		if (*end)
			ERROR("parsing operand (TTMP value)");

		if (result.value < 0 || result.value > 11)
			ERROR("invalid TTMP number (%d)", result.value);

		result.op_code = TTMP_OP.op_code + result.value;
		result.op_type = TTMP_OP;

		return result; 
	}
	else
	{
		// Parse literal constant

		if (strncmp(op_str, "0x", 2) == 0)
		{
			result.value = strtol((const char*) op_str+2, &end, 16);
		}
		else
		{
			result.value = strtol((const char*) op_str, &end, 10);
		}

		if (*end)
			ERROR("parsing operand (literal value)");

		if (result.value == 0)
		{

			result.op_code = ZERO_OP.op_code;
			result.op_type = ZERO_OP;
		}
		else if (result.value > 0 && result.value <= 64)
		{
			result.op_code = INL_POS_OP.op_code + result.value - 1;
			result.op_type = INL_POS_OP;
		}
		else if (result.value >= -16 && result.value <= -1)
		{
			result.op_code = INL_NEG_OP.op_code + (-result.value) - 1;
			result.op_type = INL_NEG_OP;
		}
		else
		{
			result.op_code = LITERAL_OP.op_code;
			result.op_type = LITERAL_OP;
		}

		return result;
	}

	// At this stage this is unreachable code

	ERROR("unsupported operand type (%s", op_str);
	return result;
}