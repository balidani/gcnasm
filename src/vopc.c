/*
 * AMD GCN ISA Assembler
 *
 * VOPC instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "vopc.h"

/**
 * Parses instructions with a VOPC encoding
 * 
 * MAGIC (7) | OP (8) | VSRC1 (8) | SSRC0 (9) | [LITERAL (32)]
 */
isa_op_code* parseVOPC(isa_instr instr, int argc, char **args)
{
	char *vsrc1_str, *src0_str;

	isa_operand *vsrc1_op, *src0_op;	// ISA operand structs
	isa_op_code *op_code;				// Generated opcode struct

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));

	if (argc < 2)
		ERROR("number of passed operands is too low");

	// Setup arguments
	src0_str	= args[0];
	vsrc1_str	= args[1];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 0;

	// SRC0
	src0_op = parseOperand(src0_str);

	if (src0_op->op_type.type == LITERAL)
		setLiteralOperand(op_code, src0_op);
	else if (src0_op->op_type.type == VGPR)
		src0_op->op_code += 256;

	op_code->code |= src0_op->op_code;	

	// VSRC1
	vsrc1_op = parseOperand(vsrc1_str);

	if (vsrc1_op->op_type.type != VGPR)
		ERROR("VSRC1 must be of VGPR type");

	op_code->code |= vsrc1_op->op_code << 9;

	free(vsrc1_op);
	free(src0_op);

	return op_code;
}