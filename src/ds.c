/*
 * AMD GCN ISA Assembler
 *
 * DS instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "ds.h"

/**
 * Parses instructions with a DS encoding
 * 
 * MAGIC (6) | OP (8) | GDS | r | OFFSET1 (8) | OFFSET0 (8)
 * | VDST (8) | DATA1 (8) | DATA0 (8) | ADDR (8)
 */
isa_op_code* parseDS(isa_instr instr, int argc, char **args)
{
	char *vdst_str = NULL, *addr_str = NULL, *data0_str = NULL, *data1_str = NULL;
	int i, j;

	isa_operand *vdst_op = NULL, *addr_op = NULL, 	// ISA operand structs
		*data0_op, *data1_op = NULL;
	isa_op_code *op_code;				// Generated opcode struct

	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 2)
		ERROR("number of passed operands is too low");

/*	if (argc == 2) {
	  if (!strncmp(instr.name, "DS_READ", 7)) {
        vdst_str    = args[0];
        addr_str    = args[1];
	  } else {
        addr_str    = args[0];
        data0_str   = args[1];
	  }
	} else */ {
	    // Setup arguments
	    vdst_str    = args[0];
	    addr_str    = args[1];
	    data0_str   = args[2];
	    data1_str   = args[3];
	}

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 1;

	if (vdst_str) {
	  // VDST
	  vdst_op = parseOperand(vdst_str);

	  if (vdst_op->op_type.type != VGPR)
	    ERROR("VDST operand must be a VGPR");

	  op_code->literal |= vdst_op->op_code << 24;
	}

	if (addr_str) {
	  // ADDR
	  addr_op = parseOperand(addr_str);

	  if (addr_op->op_type.type != VGPR)
	    ERROR("ADDR operand must be a VGPR");

	  op_code->literal |= addr_op->op_code;
	}

	if (data0_str) {
	  // DATA0
	  data0_op = parseOperand(data0_str);

	  if (data0_op->op_type.type != VGPR)
	    ERROR("DATA0 operand must be an VGPR");

	  op_code->literal |= data0_op->op_code << 8;
	}

	if (data1_str) {
	  // DATA1
	  data1_op = parseOperand(data1_str);

	  if (data1_op->op_type.type != VGPR)
	    ERROR("DATA1 operand must be an VGPR");

	  op_code->literal |= data1_op->op_code << 16;
	}

	// Parse optional parameters
	for (i = 4; i < argc; ++i)
	{
		// Convert to upper case
		for (j = 0; args[i][j]; ++j)
			args[i][j] = (char) toupper(args[i][j]);

		if (strcmp(args[i], "GDS") == 0)
			op_code->code |= (1 << 17);
		else if (strncmp(args[i], "OFFSET", strlen("OFFSET")) == 0)
		{
			if (strlen(args[i]) < strlen("OFFSET") + 2)
			{

				WARNING("wrong offset format");
				continue;
			}

			if (args[i][strlen("OFFSET") + 1] != ':')
			{
				WARNING("':' required for offset parameter");
				continue;
			}

			isa_operand *offset_op = parseOperand(args[i] 
				+ strlen("OFFSET") + 2);

			if (!isConstantOperand(offset_op))
				ERROR("non-literal value supplied for OFFSET operand");

			if (args[i][strlen("OFFSET")] == '0')
				op_code->code |= offset_op->value;
			else if (args[i][strlen("OFFSET")] == '1')
				op_code->code |= offset_op->value << 8;
			
			free(offset_op);
		}
	}

	free(vdst_op);
	free(addr_op);
	free(data1_op);
	free(data0_op);

	return op_code;
}
