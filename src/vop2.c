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

#include "vop2.h"

/**
 * Parses instructions with a VOP2 encoding
 * 
 * MAGIC (1) | OP (6) | VDST (8) | VSRC1 (8) | SSRC0 (9) | [LITERAL (32)]
 */
isa_op_code* parseVOP2(isa_instr instr, int argc, char **args)
{
	char *vdst_str, *src0_str, *vsrc1_str; 	// And *vcc_str
	int has_vcc;

	isa_operand *vdst_op, *vsrc1_op, *src0_op;	// ISA operand structs
	isa_op_code *op_code;						// Generated opcode struct

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));

	if (argc < 3)
		ERROR("number of passed operands is too low");

	// Setup arguments

	// Check for vcc
	has_vcc = !!(argc == 4);

	vdst_str	= args[0];
	//vcc_str	= args[1]; // Not used yet
	src0_str	= args[1 + has_vcc];
	vsrc1_str	= args[2 + has_vcc];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 0;

	// VDST
	vdst_op = parseOperand(vdst_str);

	if (vdst_op->op_type.type != VGPR)
		ERROR("VDST must be of VGPR type");

	op_code->code |= vdst_op->op_code << 17;

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

	free(vdst_op);
	free(vsrc1_op);
	free(src0_op);

	return op_code;
}