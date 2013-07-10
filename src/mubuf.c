/*
 * AMD GCN ISA Assembler
 *
 * MUBUF instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "mubuf.h"

/**
 * Parses instructions with a MUBUF encoding
 * 
 * MAGIC (6) | r | OP (7) | r | LDS | ADDR64 | GLC 
 * | IDXEN | OFFEN | OFFSET (12) | SOFFSET(8) | TFE
 * | SLC | r | SRSRC (5) | VDATA (8) | VADDR (8)
 */
isa_op_code* parseMUBUF(isa_instr instr, int argc, char **args)
{
	char *vdata_str, *vaddr_str, *srsrc_str, *soffset_str;
	int i, j;

	isa_operand *vdata_op, *vaddr_op, 	// ISA operand structs
		*srsrc_op, *soffset_op;	
	isa_op_code *op_code;				// Generated opcode struct

	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 4)
		ERROR("number of passed operands is too low");

	// Setup arguments
	vdata_str	= args[0];
	vaddr_str	= args[1];
	srsrc_str	= args[2];
	soffset_str	= args[3];

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

	// SOFFSET
	soffset_op = parseOperand(soffset_str);

	if (soffset_op->op_type.type != SGPR
			&& soffset_op->op_type.type != M0
			&& soffset_op->op_type.type != ZERO
			&& soffset_op->op_type.type != INL_POS
			&& soffset_op->op_type.type != INL_NEG)
		ERROR("SOFFSET operand must be an SGPR, M0 or inline constant");

	op_code->literal |= soffset_op->op_code << 24;

	// Parse optional parameters
	for (i = 4; i < argc; ++i)
	{
		// Convert to upper case
		for (j = 0; args[i][j]; ++j)
			args[i][j] = (char) toupper(args[i][j]);

		if (strcmp(args[i], "OFFEN") == 0)
			op_code->code |= (1 << 12);
		else if (strcmp(args[i], "IDXEN") == 0)
			op_code->code |= (1 << 13);
		else if (strcmp(args[i], "GLC") == 0)
			op_code->code |= (1 << 14);
		else if (strcmp(args[i], "ADDR64") == 0)
			op_code->code |= (1 << 15);
		else if (strcmp(args[i], "LDS") == 0)
			op_code->code |= (1 << 16);
		else if (strcmp(args[i], "SLC") == 0)
			op_code->literal |= (1 << 22);
		else if (strcmp(args[i], "TFE") == 0)
			op_code->literal |= (1 << 23);
		else if (strncmp(args[i], "OFFSET:", strlen("OFFSET:")) == 0)
		{
			isa_operand *offset_op = parseOperand(args[i] 
				+ strlen("OFFSET:"));

			if (!isConstantOperand(offset_op))
				ERROR("non-literal value supplied for OFFSET operand");

			op_code->code |= offset_op->value;
			
			free(offset_op);
		}

	}

	free(vdata_op);
	free(vaddr_op);
	free(srsrc_op);
	free(soffset_op);

	return op_code;
}