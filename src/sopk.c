/*
 * AMD GCN ISA Assembler
 *
 * SOPK instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "sopk.h"

/**
 * Parses instructions with a SOPK encoding
 * 
 * MAGIC (5) | OP (5) | SDST (7) | SIMM (16)
 */
isa_op_code* parseSOPK(isa_instr instr, int argc, char **args)
{
	char *sdst_str, *simm_str;

	isa_operand *sdst_op, *simm_op;	// ISA operand structs
	isa_op_code *op_code;			// Generated opcode struct

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 2)
		ERROR("number of passed operands is too low");

	// Setup arguments
	sdst_str	= args[0];
	simm_str	= args[1];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal_set = 0;

	// SDST
	sdst_op = parseOperand(sdst_str);

	if (sdst_op->op_type.type >= SDST_OPERAND_TRESHOLD)
		ERROR("incorrect value for SDST operand");

	op_code->code |= sdst_op->op_code << 16;

	// SIMM
	simm_op = parseOperand(simm_str);

	if (!isConstantOperand(simm_op))
		ERROR("non-literal value supplied for SIMM operand");

	// We use a 16 bit inline literal here
	op_code->code |= (uint16_t) simm_op->value;

	free(sdst_op);
	free(simm_op);

	return op_code;
}