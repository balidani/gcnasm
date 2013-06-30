/*
 * AMD GCN ISA Assembler
 *
 * SOPC instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "sopc.h"

/**
 * Parses instructions with a SOP2 encoding
 * 
 * MAGIC (9) | OP (7) | SSRC1 (8) | SSRC0 (8) | [LITERAL (32)]
 */
isa_op_code parseSOPC(isa_instr instr, char **args)
{
	char *ssrc1_str, *ssrc0_str;

	isa_operand ssrc1_op, ssrc0_op;	// ISA operand structs
	isa_op_code op_code;			// Generated opcode struct
	
	// Setup arguments
	ssrc0_str	= args[0];
	ssrc1_str	= args[1];

	// Parse operands
	op_code.code = instr.op_code;
	op_code.literal_set = 0;

	// SSRC0 - First source operand
	ssrc0_op = parseOperand(ssrc0_str, 8);

	if (ssrc0_op.op_type.type == VGPR)
		ERROR("incorrect value for SSRC0 operand");

	if (ssrc0_op.op_type.type == LITERAL)
		setLiteralOperand(&op_code, ssrc0_op);

	op_code.code |= ssrc0_op.op_code;	

	// SSRC1 - Second source operand
	ssrc1_op = parseOperand(ssrc1_str, 8);

	if (ssrc1_op.op_type.type == VGPR)
		ERROR("incorrect value for SSRC1 operand");

	if (ssrc1_op.op_type.type == LITERAL)
		setLiteralOperand(&op_code, ssrc1_op);

	op_code.code |= ssrc1_op.op_code << 8;

	return op_code;
}