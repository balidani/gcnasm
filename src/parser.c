/*
 * AMD GCN ISA Assembler
 *
 * Parser module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

static const char field_delimiter[] = ", \t";
static const char comment_delimiter[] = ";";

static char* getField(char **line, const char delimiter[])
{
	char *field;

	while (strlen(field = strsep(line, delimiter)) == 0);

	return field;
}

/**
 * Parses a whole ISA file line by line
 */
void parseFile(const char *input, const char *output) 
{
	FILE *in_file = fopen(input, "r");
	char line[256];

	if (in_file == NULL)
	{
		printf("Error opening file \"%s\"\n", input);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), in_file))
	{
		parseLine(line);
	}

	fclose(in_file);
}

/**
 * Parses a single line of ISA assembly
 *
 * (Work in progress, it only prints the output to stdout yet)
 */
void parseLine(char *line)
{
	char *line_strip;
	char *token;
	int i;

	// Strip comments
	line_strip = getField(&line, comment_delimiter);
	
	// Get instruction token
	token = getField(&line_strip, field_delimiter);
	
	for (i = 0; i < isa_instr_count; ++i)
	{
		if (strcmp(isa_instr_list[i].name, token) == 0)
		{
			break;
		}
	}

	if (i == isa_instr_count)
	{
		printf("Unrecognized instruction '%s'\n", token);
		return;
	}

	printf("%s \t", isa_instr_list[i].name);

	// Parse encoding
	switch (isa_instr_list[i].encoding)
	{
		case SOP2:
			parseSOP2(isa_instr_list[i], line_strip);
			break;
		default:
			printf("Unsupported encoding type for instruction '%s'", 
				isa_instr_list[i].name);
			break;
	}
}

/**
 * Parses instructions with a SOP2 encoding
 * 
 * MAGIC (2) | OP (7) | SDST (7) | SSRC1 (8) | SSRC0 (8) | [LITERAL (32)]
 */
void parseSOP2(isa_instr instr, char *line)
{
	char *dst_str, *src1_str, *src0_str;	// Raw strings
	isa_operand dst_op, src1_op, src0_op;	// ISA operand structs

	uint32_t op_code = instr.op_code;
	uint32_t op_lit = 0;
	int op_lit_set = 0;

	// Parse dst, src1, src0
	while (strlen(dst_str = strsep(&line, field_delimiter)) == 0);
	while (strlen(src1_str = strsep(&line, field_delimiter)) == 0);
	while (strlen(src0_str = strsep(&line, field_delimiter)) == 0);
	
	// Trim src0
	if (src0_str[strlen(src0_str)-1] == '\n')
		src0_str[strlen(src0_str)-1] = '\0';

	// Parse operands
	// These will probably need to be moved to separate functions

	// SDST
	dst_op = parseOperand(dst_str);

	if (src1_op.type == LITERAL)
	{
		// Error
	}

	op_code |= dst_op.op_code << 16;

	// SSRC1
	src1_op = parseOperand(src1_str);

	if (src1_op.type == LITERAL)
	{
		if (op_lit_set)
		{
			// Error
		}

		op_lit_set = 1;
		op_lit = src1_op.value;
	}

	op_code |= src1_op.op_code << 8;

	// SSRC0
	src0_op = parseOperand(src0_str);

	if (src0_op.type == LITERAL)
	{
		if (op_lit_set)
		{
			// Error
		}

		op_lit_set = 1;
		op_lit = src0_op.value;
	}

	op_code |= src0_op.op_code;	

	printf("0x%08x", op_code);

	if (op_lit_set)
	{
		printf(" 0x%08x\n", op_lit);
	}
	else
	{
		printf("\n");
	}
}

/**
 * Parses the SDST operand for instructions
 * 
 * Only the SOP2 format is supported for now
 */
isa_operand parseOperand(char *dst)
{
	isa_operand result;
	char *end;
	
	// Only SGPRs and literals supported for now

	if (tolower(dst[0]) == 's')
	{
		result.value = strtol((const char*) dst+1, &end, 10);

		if (!(*end))
		{
			result.op_code = result.value;
			result.type = SGPR;

			return result;
		} 

	}
	else
	{
		if (strncmp(dst, "0x", 2) == 0)
		{
			result.value = strtol((const char*) dst+2, &end, 16);
		}
		else
		{
			result.value = strtol((const char*) dst, &end, 10);
		}

		if (!(*end))
		{
			if (result.value >= 0 && result.value <= 64)
			{
				result.op_code = 128 + result.value;
				result.type = CONST_POS;
			}
			else if (result.value >= -16 && result.value <= -1)
			{
				result.op_code = 192 + (-result.value);
				result.type = CONST_NEG;
			}
			else
			{
				result.op_code = 0xFF;
				result.type = LITERAL;
			}

			return result;
		}
	}

	result.op_code = 0;
	result.type = ERROR;
	result.value = 0;

	return result;
}