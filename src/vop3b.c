/*
 * AMD GCN ISA Assembler
 *
 * VOP3b instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "vop3b.h"

/**
 * Finds and removes NEG() tags before the operand is processed
 *
 * Returns 0x00 if neither is found
 * Returns 0x01 if NEG is found
 *
 * Ugly code duplication from format VOP3a
 */
static char preProcessOp(char **op_str)
{
	char result = 0x00;
	int i;

	// Convert to upper case
	for (i = 0; (*op_str)[i]; ++i)
		(*op_str)[i] = (char) toupper((*op_str)[i]);

	if (strncmp(*op_str, "NEG(", strlen("NEG(")) == 0)
	{
		if (result & 0x10)
			WARNING("ABS will only be applied _before_ NEG is");

		result |= (char) 0x01;

		*op_str += strlen("NEG(");

		if ((*op_str)[strlen(*op_str) - 1] == ')')
			(*op_str)[strlen(*op_str) - 1] = '\0';
		else
			WARNING("unclosed NEG modifier");
	}

	return result;
}

/**
 * Parses instructions with a VOP3b encoding
 * 
 * MAGIC (6) | OP (9) | r (2) | SDST (7) | VDST (8)
 * | NEG (3) | OMOD (2) | SRC2 (9) | SRC1 (9) | SRC0 (9)
 */
isa_op_code* parseVOP3b(isa_instr instr, int argc, char **args)
{
	char *vdst_str = NULL, *sdst_str = NULL, *src0_str, *src1_str, *src2_str;
	char omod_value, neg_value, tag_result;
	int i, j;

	isa_operand *vdst_op = NULL, *sdst_op = NULL,		// ISA operand structs
		*src0_op, *src1_op, *src2_op; 			
	isa_op_code *op_code;				// Generated opcode struct

	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	omod_value = 0;
	neg_value = 0;

	if (argc < 3)
		ERROR("number of passed operands is too low");

	if (argc < 4) {
      // Setup arguments
      vdst_str  = args[0];
      src0_str  = args[1];
      src1_str  = args[2];
	} else {
	  // Setup arguments
	  vdst_str	= args[0];
	  sdst_str	= args[1];
	  src0_str	= args[2];
	  src1_str	= args[3];
	}
	if (argc > 4)
		src2_str = args[4];
	
	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 1;

	if (vdst_str) {
	  // VDST
	  vdst_op = parseOperand(vdst_str);

	  op_code->code |= vdst_op->op_code;
	}

	if (sdst_str) {
	  // SDST
	  sdst_op = parseOperand(sdst_str);

	  if (sdst_op->op_type.type >= SDST_OPERAND_TRESHOLD)
	    ERROR("incorrect value for SDST operand");

	  op_code->code |= (sdst_op->op_code << 8);
	}

	// SRC0
	tag_result = preProcessOp(&src0_str);
	if (tag_result & 0x01)
		neg_value &= 1;
	
	src0_op = parseOperand(src0_str);

	if (src0_op->op_type.type == LITERAL)
	{
		ERROR("SRC0 operand cannot be a (long) literal value");
	}
	else if (src0_op->op_type.type == VGPR)
	{
		src0_op->op_code += 256;
	}

	op_code->literal |= src0_op->op_code;
	
	// SRC1
	tag_result = preProcessOp(&src1_str);
	if (tag_result & 0x01)
		neg_value &= 1 << 1;

	src1_op = parseOperand(src1_str);

	if (src1_op->op_type.type == LITERAL)
	{
		ERROR("SRC1 operand cannot be a (long) literal value");
	}
	else if (src1_op->op_type.type == VGPR)
	{
		src1_op->op_code += 256;
	}

	op_code->literal |= (src1_op->op_code << 9);

	// SRC2
	if (argc > 4)
	{
		tag_result = preProcessOp(&src2_str);
		if (tag_result & 0x01)
			neg_value &= 1 << 2;

		src2_op = parseOperand(src2_str);
	}
	else
	{
		src2_str = (char *) calloc(strlen("s0") + 1, sizeof(char));
		strncpy(src2_str, "s0", 2);

		src2_op = parseOperand(src2_str);
		free(src2_str);
	}

	if (src2_op->op_type.type == LITERAL)
	{
		ERROR("SRC2 operand cannot be a (long) literal value");
	}
	else if (src2_op->op_type.type == VGPR)
	{
		src2_op->op_code += 256;
	}

	op_code->literal |= (src2_op->op_code << 18);

	for (i = 3 + !!(argc > 3); i < argc; ++i)
	{
		// Convert to upper case
		for (j = 0; args[i][j]; ++j)
			args[i][j] = (char) toupper(args[i][j]);

		if (strncmp(args[i], "DIV(", strlen("DIV(")) == 0)
		{
			switch (args[i][strlen("DIV(")])
			{
				case '1':
					omod_value = 0;
					break;
				case '2':
					omod_value = 3;
					break;
				default:
					break;
			}

			if (args[i][strlen("DIV(") + 1] != ')')
				WARNING("unclosed DIV modifier");
			
		}
		else if (strncmp(args[i], "MUL(", strlen("MUL(")) == 0)
		{
			switch (args[i][strlen("MUL(")])
			{
				case '1':
					omod_value = 0;
					break;
				case '2':
					omod_value = 1;
					break;
				case '4':
					omod_value = 2;
					break;
			}

			if (args[i][strlen("MUL(") + 1] != ')')
				WARNING("unclosed MUL modifier");
		}

	}

	op_code->literal |= ((uint32_t) omod_value << 27);
	op_code->literal |= ((uint32_t) neg_value << 29);

	free(vdst_op);
	free(sdst_op);
	free(src0_op);
	free(src1_op);

	if (src2_op != NULL)
		free(src2_op);

	return op_code;
}
