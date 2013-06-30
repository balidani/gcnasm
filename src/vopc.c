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
isa_op_code parseVOPC(isa_instr instr, char **args)
{
	char *src0_str, *vsrc1_str;

	isa_operand vsrc1_op, src0_op;	// ISA operand structs
	isa_op_code op_code;			// Generated opcode struct

	// Setup arguments
	src0_str	= args[0];
	vsrc1_str	= args[1];

	// Parse operands
	op_code.code = instr.op_code;
	op_code.literal_set = 0;

	// SRC0
	src0_op = parseOperand(src0_str, 9);

	if (src0_op.op_type.type == LITERAL)
		setLiteralOperand(&op_code, src0_op);

	op_code.code |= src0_op.op_code;	

	// VSRC1
	vsrc1_op = parseOperand(vsrc1_str, 8);

	if (vsrc1_op.op_type.type != VGPR)
		ERROR("VSRC1 must be of VGPR type");

	op_code.code |= vsrc1_op.op_code << 9;

	return op_code;
}