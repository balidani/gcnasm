/*
 * AMD GCN ISA Assembler
 *
 * SOPP instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "sopp.h"

/**
 * Parses instructions with a SOPP encoding
 * 
 * MAGIC (9) | OP (7) | SIMM (16)
 */
isa_op_code* parseSOPP(isa_instr instr, int argc, char **args)
{
	char *simm_str;

	isa_operand *simm_op;	// ISA operand structs
	isa_op_code *op_code;	// Generated opcode struct

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 1)
		ERROR("number of passed operands is too low");

	// Setup arguments
	simm_str	= args[0];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 0;

	// SIMM
	simm_op = parseOperand(simm_str);

	if (!isConstantOperand(simm_op))
		ERROR("non-literal value supplied for SIMM operand");

	// We use a 16 bit inline literal here
	op_code->code |= (uint16_t) simm_op->value;

	free(simm_op);

	return op_code;
}