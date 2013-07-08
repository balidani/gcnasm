/*
 * AMD GCN ISA Assembler
 *
 * SOP2 instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "sop2.h"

/**
 * Parses instructions with a SOP2 encoding
 * 
 * MAGIC (2) | OP (7) | SDST (7) | SSRC1 (8) | SSRC0 (8) | [LITERAL (32)]
 */
isa_op_code* parseSOP2(isa_instr instr, int argc, char **args)
{
	char *sdst_str, *ssrc1_str, *ssrc0_str;

	isa_operand *sdst_op, *ssrc1_op, *ssrc0_op;	// ISA operand structs
	isa_op_code *op_code;						// Generated opcode struct
	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 3)
		ERROR("number of passed operands is too low");

	// Setup arguments
	sdst_str	= args[0];
	ssrc0_str	= args[1];
	ssrc1_str	= args[2];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 0;

	// SDST
	sdst_op = parseOperand(sdst_str);

	if (sdst_op->op_type.type >= SDST_OPERAND_TRESHOLD)
		ERROR("incorrect value for SDST operand");

	op_code->code |= sdst_op->op_code << 16;

	// SSRC0 - First source operand
	ssrc0_op = parseOperand(ssrc0_str);

	if (ssrc0_op->op_type.type == VGPR)
		ERROR("incorrect value for SSRC0 operand");

	if (ssrc0_op->op_type.type == LITERAL)
		setLiteralOperand(op_code, ssrc0_op);

	op_code->code |= ssrc0_op->op_code;	

	// SSRC1 - Second source operand
	ssrc1_op = parseOperand(ssrc1_str);

	if (ssrc1_op->op_type.type == VGPR)
		ERROR("incorrect value for SSRC1 operand");

	if (ssrc1_op->op_type.type == LITERAL)
		setLiteralOperand(op_code, ssrc1_op);

	op_code->code |= ssrc1_op->op_code << 8;

	free(sdst_op);
	free(ssrc1_op);
	free(ssrc0_op);

	return op_code;
}