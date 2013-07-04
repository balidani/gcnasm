/*
 * AMD GCN ISA Assembler
 *
 * SMRD instruction parser
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

#include "smrd.h"

/**
 * Parses instructions with a SMRD encoding
 * 
 * MAGIC (5) | OP (5) | SDST (7) | SBASE (6) | IMM (1) | OFFSET (8) | 
 * [LITERAL (32)]
 */
isa_op_code* parseSMRD(isa_instr instr, int argc, char **args)
{
	char *sdst_str, *sbase_str, *offset_str;

	isa_operand *sdst_op, *sbase_op, *offset_op;	// ISA operand structs
	isa_op_code *op_code;							// Generated opcode struct
	
	if (argc < 3)
		ERROR("number of passed operands is too low");

	// Setup arguments
	sdst_str	= args[0];
	sbase_str	= args[1];
	offset_str	= args[2];

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal_set = 0;

	// SDST
	sdst_op = parseOperand(sdst_str);

	if (sdst_op->op_type.type != SGPR 
			&& sdst_op->op_type.type != VCC_LO 
			&& sdst_op->op_type.type != VCC_HI)
		ERROR("wrong type of operand for SDST in the SMRD format");

	op_code->code |= sdst_op->op_code << 15;

	// SBASE
	sbase_op = parseOperand(sbase_str);

	if (sbase_op->op_type.type != SGPR)
		ERROR("wrong type of operand for SBASE in the SMRD format");

	// This field is missing the LSB
	// It can only be a multiple of two
	sbase_op->op_code >>= 1;

	op_code->code |= sbase_op->op_code << 9;

	// OFFSET
	offset_op = parseOperand(offset_str);

	if (offset_op->op_type.type != SGPR)
	{
		// If it's not an SGPR then it is a literal byte value
		// It is easier to simply reparse, because inline constant literals
		// are only supported up to 64

		char *end;

		if (strncmp(offset_str, "0x", 2) == 0)
		{
			offset_op->op_code = strtol((const char*) offset_str+2, &end, 16);
		}
		else
		{
			offset_op->op_code = strtol((const char*) offset_str, &end, 10);
		}

		// Also set the IMM bit
		op_code->code |= (1 << 8);
	}

	op_code->code |= offset_op->op_code;

	free(sdst_op);
	free(sbase_op);
	free(offset_op);

	return op_code;
}