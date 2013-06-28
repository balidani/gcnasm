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

static char* getField(char **line, const char delimiter[])
{
	char *field;

	while (1)
	{
		field = strsep(line, delimiter);
		
		if (field == NULL)
			break;

		if (strlen(field) > 0)
			break;
	}

	return field;
}

/**
 * Parses a whole ISA file line by line
 */
void parseFile(const char *input, const char *output) 
{
	FILE *in_file;
	char line[256];

	in_file = fopen(input, "r");

	if (in_file == NULL)
		ERROR("opening file \"%s\"", input);

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

	// Special case for commented lines
	if (line[0] == comment_delimiter[0])
		return;

	// Strip comments
	line_strip = getField(&line, comment_delimiter);

	// Get instruction token
	token = getField(&line_strip, field_delimiter);

	// Skip empty lines
	if (token == NULL)
		return;
	
	for (i = 0; i < isa_instr_count; ++i)
		if (strcmp(isa_instr_list[i].name, token) == 0)
			break;

	if (i == isa_instr_count)
		ERROR("unrecognized instruction '%s'\n", token);

	printf("%s \t", isa_instr_list[i].name);

	// Parse encoding
	switch (isa_instr_list[i].encoding)
	{
		case SOP2: 
			{
				// Raw strings
				char **args;
				char *dst_str, *src1_str, *src0_str;	

				// Parse dst, src1, src0
				while (strlen(dst_str = strsep(&line_strip, field_delimiter)) == 0);
				while (strlen(src1_str = strsep(&line_strip, field_delimiter)) == 0);
				while (strlen(src0_str = strsep(&line_strip, field_delimiter)) == 0);

				args = (char **) calloc(3, sizeof(char *));
				args[0] = dst_str;
				args[1] = src1_str;
				args[2] = src0_str;

				parseSOP2Instruction(isa_instr_list[i], args);
			}
			break;
		case VOP2:
			{
				WARNING("VOP2 is not supported yet\n");
			} 
			break;
		default:
			printf("Unsupported encoding type for instruction '%s'", 
				isa_instr_list[i].name);
			break;
	}
}