/*
 * AMD GCN ISA Assembler
 *
 * MIMG instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "mimg.h"

/**
 * Parses instructions with a MIMG encoding
 * 
 * MAGIC (6) | S | OP (7) | L | T | R | D | G | U | DMASK
 * | r | r | SSAMP (5) | SRSRC(5) | VDATA(8) | VADDR(8)
 */
isa_op_code* parseMIMG(isa_instr instr, int argc, char **args)
{
	char *vdata_str, *vaddr_str, *ssamp_str, *srsrc_str;
	int dmask_parsed;
	int i, j;

	isa_operand *vdata_op, *vaddr_op, 	// ISA operand structs
		*ssamp_op, *srsrc_op;	
	isa_op_code *op_code;				// Generated opcode struct

	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 4)
		ERROR("number of passed operands is too low");

	// Setup arguments
	vdata_str	= args[0];
	vaddr_str	= args[1];
	srsrc_str	= args[2];
	ssamp_str	= args[3];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 1;

	// VDATA
	vdata_op = parseOperand(vdata_str);

	if (vdata_op->op_type.type != VGPR)
		ERROR("VDATA operand must be a VGPR");

	op_code->literal |= vdata_op->op_code << 8;

	// VADDR
	vaddr_op = parseOperand(vaddr_str);

	if (vaddr_op->op_type.type != VGPR)
		ERROR("VADDR operand must be a VGPR");

	op_code->literal |= vaddr_op->op_code;

	// SRSRC
	srsrc_op = parseOperand(srsrc_str);

	// This field is missing 2 bits from LSB
	srsrc_op->op_code >>= 2;

	if (srsrc_op->op_type.type != SGPR)
		ERROR("SRSRC operand must be an SGPR");

	op_code->literal |= srsrc_op->op_code << 16;

	// SSAMP
	ssamp_op = parseOperand(ssamp_str);

	if (ssamp_op->op_type.type != SGPR)
		ERROR("SSAMP operand must be an SGPR");

	// This field is also missing 2 bits from the LSB
	ssamp_op->op_code >>= 2;

	op_code->literal |= ssamp_op->op_code << 21;

	// Parse optional parameters
	dmask_parsed = 0;

	for (i = 4; i < argc; ++i)
	{
		// Convert to upper case
		for (j = 0; args[i][j]; ++j)
			args[i][j] = (char) toupper(args[i][j]);

		if (strcmp(args[i], "SLC") == 0)
			op_code->code |= (1 << 25);
		else if (strcmp(args[i], "LWE") == 0)
			op_code->code |= (1 << 17);
		else if (strcmp(args[i], "TFE") == 0)
			op_code->code |= (1 << 16);
		else if (strcmp(args[i], "R128") == 0)
			op_code->code |= (1 << 15);
		else if (strcmp(args[i], "DA") == 0)
			op_code->code |= (1 << 14);
		else if (strcmp(args[i], "GLC") == 0)
			op_code->code |= (1 << 13);
		else if (strcmp(args[i], "UNORM") == 0)
			op_code->code |= (1 << 12);
		else if (!dmask_parsed)
		{
			isa_operand *dmask_op = parseOperand(args[i]);

			if (!isConstantOperand(dmask_op))
				ERROR("non-literal value supplied for DMASK operand");

			// Only 4 bits are needed
			dmask_op->value &= 0xF;

			op_code->code |= (dmask_op->value) << 8;
			dmask_parsed = 1;
			
			free(dmask_op);
		}

	}

	free(vdata_op);
	free(vaddr_op);
	free(srsrc_op);
	free(ssamp_op);

	return op_code;
}