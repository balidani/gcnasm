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
static const char comment_delimiter[] = ";\n";

/**
 * Parses a line or operand using strsep
 * and a list of delimiters.
 *
 * Removes any trailing new-line
 */
char* parseField(char **line, const char delimiter[])
{
	char *field;

	while (1)
	{
		field = strsep(line, delimiter);
		
		if (field == NULL)
			return field;

		if (strlen(field) > 0)
			break;
	}

	// Strip trailing new-line
	if (field[strlen(field) - 1] == '\n')
		field[strlen(field) - 1] =  '\0';

	return field;
}

/**
 * Parses a whole ISA file line by line
 */
void parseFile(const char *input, const char *output) 
{
	FILE *in_file, *out_file;
	char line[256];

	uint32_t *microcode;
	int microcode_ptr;
	int line_count;
	int i;

	// Count lines to estimate microcode size
	line_count = 0;
	in_file = fopen(input, "r");

	if (in_file == NULL)
		ERROR("opening file \"%s\"", input);

	while (fgets(line, sizeof(line), in_file))
	{
		line_count++;
	}

	fclose(in_file);

	// Allocate space for microcode
	microcode = (uint32_t *) calloc((size_t) line_count * 2, 
		sizeof (uint32_t));
	microcode_ptr = 0;

	// Parse each line
	in_file = fopen(input, "r");

	if (in_file == NULL)
		ERROR("opening file \"%s\"", input);

	while (fgets(line, sizeof(line), in_file))
	{
		isa_op_code *result;

		result = parseLine(line);

		if (result == NULL)
			continue;

		// printf("%08x\n", result->code);

		microcode[microcode_ptr++] = result->code;

		if (result->literal_set)
		{
			microcode[microcode_ptr++] = result->literal;
			// printf("%08x\n", result->literal);
		}

		free(result);
	}

	fclose(in_file);

	// Write results to output
	out_file = fopen(output, "w");

	if (out_file == NULL)
		ERROR("opening file \"%s\"", output);

	for (i = 0; i < microcode_ptr; ++i)
	{
		uint32_t src = microcode[i];

		if (fwrite(&src, sizeof(uint32_t), 1, out_file) == 0)
			ERROR("writing file \"%s\"", output);
	}

	fclose(out_file);

	free(microcode);
}

/**
 * Parses a single line of ISA assembly
 *
 * (Work in progress, it only prints the output to stdout yet)
 */
isa_op_code* parseLine(char *line)
{
	isa_op_code *result;
	char *line_strip;
	char *token;

	char **args;
	int argc;
	int max_op_count;
	int i, j;


	// Special case for commented lines
	if (line[0] == comment_delimiter[0])
		return NULL;

	// Strip comments
	line_strip = parseField(&line, comment_delimiter);

	// Get instruction token
	token = parseField(&line_strip, field_delimiter);

	// Skip empty lines
	if (token == NULL)
		return NULL;

	// Convert token to uppercase
	for (i = 0; token[i]; ++i)
		token[i] = (char) toupper(token[i]);

	for (i = 0; i < isa_instr_count; ++i)
		if (strcmp(isa_instr_list[i].name, token) == 0)
			break;

	if (i == isa_instr_count)
		ERROR("unrecognized instruction '%s'", token);

	// Find encoding format
	for (j = 0; j < isa_format_count; ++j)
		if (isa_instr_list[i].encoding == isa_format_list[j].encoding)
			break;

	if (j == isa_format_count)
		ERROR("unrecognized format for the instruction '%s'", token);

	max_op_count = isa_format_list[j].max_op_count;

	// Allocate space for maximum number of operand pointers
	args = (char **) calloc((size_t) max_op_count, sizeof(char *));
	
	// Parse operand tokens
	argc = 0;
	while (line_strip != NULL)
	{
		char *res = parseField(&line_strip, field_delimiter);
		
		if (res != NULL)
			args[argc++] = res;
	}
	
	result = NULL;

	switch (isa_instr_list[i].encoding)
	{
		case SOP2: 
			result = parseSOP2(isa_instr_list[i], argc, args);
			break;
		case SOPK: 
			result = parseSOPK(isa_instr_list[i], argc, args);
			break;
		case SOP1:
			result = parseSOP1(isa_instr_list[i], argc, args);
			break;
		case SOPC:
			result = parseSOPC(isa_instr_list[i], argc, args);
			break;
		case SOPP:
			result = parseSOPP(isa_instr_list[i], argc, args);
			break;
		case SMRD:
			result = parseSMRD(isa_instr_list[i], argc, args);
			break;
		case VOP2:
			result = parseVOP2(isa_instr_list[i], argc, args);
			break;
		case VOP1:
			result = parseVOP1(isa_instr_list[i], argc, args);
			break;
		case VOPC:
			result = parseVOPC(isa_instr_list[i], argc, args);
			break;
		case MUBUF:
			result = parseMUBUF(isa_instr_list[i], argc, args);
			break;
		case MTBUF:
			result = parseMTBUF(isa_instr_list[i], argc, args);
			break;
		default:
			WARNING("unsupported encoding type for instruction '%s'", 
				isa_instr_list[i].name);
			break;
	}

	free(args);

	return result;
}
