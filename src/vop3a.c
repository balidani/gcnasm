/*
 * AMD GCN ISA Assembler
 *
 * VOP3a instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "vop3a.h"

/**
 * Parses instructions with a VOP3a encoding
 * 
 * MAGIC (6) | OP (9) | r (5) | CLAMP | ABS (3) | VDST (8)
 * | NEG (3) | OMOD (2) | SRC2 (9) | SRC1 (9) | SRC0 (9)
 */
isa_op_code* parseVOP3a(isa_instr instr, int argc, char **args)
{
	char *vdst_str, *src0_str, *src1_str, *src2_str;
	char omod_value, neg_value, abs_value, tag_result;
	int i, j;

	isa_operand *vdst_op, *src0_op, 	// ISA operand structs
		*src1_op, *src2_op; 			
	isa_op_code *op_code;				// Generated opcode struct
	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	omod_value = 0;
	neg_value = 0;
	abs_value = 0;

	if (argc < 3)
		ERROR("number of passed operands is too low");

	// Setup arguments
	vdst_str	= args[0];
	src0_str	= args[1];
	src1_str	= args[2];

	if (argc > 3)
		src2_str = args[3];
	
	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 1;

	// VDST
	vdst_op = parseOperand(vdst_str);

	if (vdst_op->op_type.type != VGPR)
		ERROR("VDST operand must be a VGPR");

	op_code->code |= vdst_op->op_code;

	// SRC0
	tag_result = preProcessOp(&src0_str);
	if (tag_result & 0x01)
		neg_value &= 1;
	if (tag_result & 0x10)
		abs_value &= 1;
	
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
	if (tag_result & 0x10)
		abs_value &= 1 << 1;

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
	if (argc > 3)
	{
		tag_result = preProcessOp(&src2_str);
		if (tag_result & 0x01)
			neg_value &= 1 << 2;
		if (tag_result & 0x10)
			abs_value &= 1 << 2;

		src2_op = parseOperand(src2_str);
	}
	else
	{
		src2_str = (char *) calloc(2, sizeof(char));
		strncpy(src2_str, "0", 1);
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

		if (strcmp(args[i], "CLAMP") == 0)
			op_code->code |= (1 << 11);
		else if (strncmp(args[i], "DIV(", strlen("DIV(")) == 0)
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
	op_code->code |= ((uint32_t) abs_value << 8);

	free(vdst_op);
	free(src0_op);
	free(src1_op);

	if (src2_op != NULL)
		free(src2_op);

	return op_code;
}

/**
 * Finds and removes NEG() and ABS() tags before the operand is processed
 *
 * Returns 0x00 if neither is found
 * Returns 0x01 if NEG is found
 * Returns 0x10 if ABS is found
 * Returns 0x11 if both is found
 */
char preProcessOp(char **op_str)
{
	char result = 0x00;
	int found_tag;
	int i;

	// Convert to upper case
	for (i = 0; (*op_str)[i]; ++i)
		(*op_str)[i] = (char) toupper((*op_str)[i]);

	do 
	{
		found_tag = 0;

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

			found_tag = 1;
		}
		else if (strncmp(*op_str, "ABS(", strlen("ABS(")) == 0)
		{
			result |= (char) 0x10;

			*op_str += strlen("ABS(");

			if ((*op_str)[strlen(*op_str) - 1] == ')')
				(*op_str)[strlen(*op_str) - 1] = '\0';
			else
				WARNING("unclosed ABS modifier");

			found_tag = 1;
		}
	} 
	while (found_tag);

	return result;
}