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

static format_parser format_parser_list[] = 
{
	{NONE,		NULL},
	{SOP1,		&parseSOP1},
	{SOP2,		&parseSOP2},
	{SOPK,		&parseSOPK},
	{SOPC,		&parseSOPC},
	{SOPP,		&parseSOPP},
	{SMRD,		&parseSMRD},
	{VOP1,		&parseVOP1},
	{VOP2,		&parseVOP2},
	{VOP3a,		&parseVOP3a},
	{VOP3b,		&parseVOP3b},
	{VOPC,		&parseVOPC},
	{VINTRP,	NULL},
	{DS,		&parseDS},
	{EXP,		NULL},
	{MUBUF,		&parseMUBUF},
	{MTBUF,		&parseMTBUF},
	{MIMG,		&parseMIMG}
};

/**
 * Finds the alternative parser function for an instruction
 */
static int findParser(isa_instr_enc enc)
{
	int length, i;

	length = sizeof(format_parser_list) / sizeof(format_parser);
	for (i = 0; i < length; ++i)
		if (format_parser_list[i].encoding == enc)
			break;

	if (i == length)
		return -1;

	return i;
}

/**
 * Executes the alternative parser function for an instruction
 */
static isa_op_code* parseAlternate(isa_instr instr, int argc, char **args)
{
	isa_instr instr_copy;
	isa_op_code* result;
	int fun_id = findParser(instr.alt_encoding);

	// Replace op code before the parsing happens
	memcpy(&instr_copy, &instr, sizeof(isa_instr));
	instr_copy.op_code = instr_copy.alt_op_code;

	if (fun_id >= 0)
		result = format_parser_list[fun_id].parser(
			instr_copy, argc, args);
	else
		ERROR("wrong number of arguments for alternative format");

	return result;
}

/**
 * Parses a whole ISA file line by line
 */
void parseFile(const char *input, const char *output) 
{
	FILE *in_file, *out_file;
	char line[256];

	int line_count;
	unsigned int i;

	// Count lines to estimate microcode size
	line_count = 0;
	in_file = fopen(input, "r");

	if (in_file == NULL)
		ERROR("opening file \"%s\"", input);

	while (fgets(line, sizeof(line), in_file))
	{
		line_count++;
	}

	// Allocate space for microcode
	microcode.code = (uint32_t *) calloc((size_t) line_count * 2, 
		sizeof (uint32_t));
	microcode.ptr = 0;

	// Pre-parse and find labels
	rewind(in_file);

	while (fgets(line, sizeof(line), in_file))
	{
		parseLabel(line);
	}

	// Parse each line
	rewind(in_file);

	while (fgets(line, sizeof(line), in_file))
	{
		isa_op_code *result;

		result = parseLine(line);
		line_number++;

		if (result == NULL)
			continue;

		microcode.code[microcode.ptr++] = result->code;

		if (result->literal_set)
			microcode.code[microcode.ptr++] = result->literal;

		free(result);
	}

	fclose(in_file);

	// Post process label occurrences
	processOccurrence();

	// Write results to output
	out_file = fopen(output, "w");

	if (out_file == NULL)
		ERROR("opening file \"%s\"", output);

	for (i = 0; i < microcode.ptr; ++i)
	{
		uint32_t src = microcode.code[i];

		if (fwrite(&src, sizeof(uint32_t), 1, out_file) == 0)
			ERROR("writing file \"%s\"", output);
	}

	fclose(out_file);

	free(microcode.code);
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
	unsigned int i, j;

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

	// If the instruction is not recognized, try to parse a define
	if (strncmp(token, "#DEFINE", strlen("#DEFINE")) == 0)
	{
		char *tag = parseField(&line_strip, field_delimiter);
		char *operand = parseField(&line_strip, field_delimiter);

		addAlias(tag, operand);

		return NULL;
	}

	// If a define is not recognized, try to parse a label
	if (i == isa_instr_count)
	{
		for (i = 0; i < label_count; ++i)
			if (strncmp(label_list[i].name, token, 
					strlen(label_list[i].name)) == 0)
				break;

		if (i == label_count)
		{
			WARNING("unrecognized instruciton '%s'", token);
		}
		else
		{
			// Add the microcode pointer here, since it isn't known 
			// when pre-parsing happens
			label_list[i].instr_ptr = microcode.ptr;
		}

		return NULL;
	}

	max_op_count = -1;

	// Find (max) encoding format
	for (j = 0; j < isa_format_count; ++j)
		if (isa_instr_list[i].encoding == isa_format_list[j].encoding
			|| isa_instr_list[i].alt_encoding == isa_format_list[j].encoding)
			if (isa_format_list[j].max_op_count > max_op_count)
				max_op_count = isa_format_list[j].max_op_count;

	if (max_op_count < 0)
		ERROR("unrecognized format for the instruction '%s'", token);

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
		#define CASE_PARSE(type) case type: result = parse##type(isa_instr_list[i], argc, args); break;
		CASE_PARSE(SOP2)
		CASE_PARSE(SOPK)
		CASE_PARSE(SOP1)
		CASE_PARSE(SOPC)
		CASE_PARSE(SOPP)
		CASE_PARSE(SMRD)
		CASE_PARSE(VOP2)
		CASE_PARSE(VOP1) // VOP3a/b support still needed
		//CASE_PARSE(VOPC)
		CASE_PARSE(VOP3a)
		CASE_PARSE(VOP3b)
		CASE_PARSE(DS)
		CASE_PARSE(MUBUF)
		CASE_PARSE(MTBUF)
		CASE_PARSE(MIMG)
		#undef CASE_PARSE

		case VOPC:
			result = parseAlternate(isa_instr_list[i], argc, args);
			break;
		default:
			WARNING("unsupported encoding type for instruction '%s'", 
				isa_instr_list[i].name);
			break;
	}

	free(args);

	return result;
}

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