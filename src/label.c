/*
 * AMD GCN ISA Assembler
 *
 * Label handling
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "label.h"

label *label_list;
unsigned int label_count;

label_occurrence *occurrence_list;
unsigned int occurrence_count;

// A reasonal upper limit for labels
// TODO: in later versions an unlimited number 
// of labels should be supported
static const unsigned int max_label = 256;
static const unsigned int max_occurrence = 1024;

void initLabel()
{
	label_list = (label *) calloc((size_t) max_label, sizeof(label));
	if (!label_list)
		ERROR("cannot allocate memory for label_list");
	label_count = 0;

	occurrence_list = (label_occurrence *) calloc((size_t) max_occurrence, 
		sizeof(label_occurrence));
	if (!occurrence_list)
		ERROR("cannot allocate memory for occurrence_list");
	occurrence_count = 0;
}

void clearLabel()
{
	unsigned int i;

	for (i = 0; i < label_count; ++i)
		if (label_list[i].name != NULL)
			free(label_list[i].name);
	
	free(label_list);
	free(occurrence_list);
}

void addLabel(char *name, uint32_t instr_ptr)
{
	if (label_count == max_label)
		ERROR("maximum label count reached");

	label_list[label_count].name = (char *) calloc(strlen(name) + 1, 
		sizeof(char));

	strncpy(label_list[label_count].name, name, strlen(name));

	label_list[label_count].instr_ptr = instr_ptr;
	label_count++;
}

void addOccurrence(unsigned int id, uint32_t ptr)
{
	if (occurrence_count == max_occurrence)
		ERROR("maxmimum occurrence count reached");

	occurrence_list[occurrence_count].label_id = id;
	occurrence_list[occurrence_count].instr_ptr = ptr;
	occurrence_count++;
}

void parseLabel(char *token)
{
	unsigned int i;
	
	for (i = 0; token[i]; ++i)
		if (token[i] == ' ' || token[i] == '\t' || token[i] == '\n')
			break;

	if (i < 2 || token[i-1] != ':')
		return;

	token[i-1] = '\0';

	// Convert to upper case
	for (i = 0; token[i]; ++i)
		token[i] = (char) toupper(token[i]);

	addLabel(token, 0xFFFFFFFF);
}

void processOccurrence()
{
	unsigned int i;

	for (i = 0; i < occurrence_count; ++i)
	{
		label_occurrence occurrence = occurrence_list[i];
		label current_label = label_list[occurrence.label_id];

		uint32_t current_ptr = occurrence.instr_ptr + 1;
		uint32_t label_ptr = current_label.instr_ptr;
		uint16_t value;

		if (label_ptr - current_ptr > 0xFFFF 
				&& current_ptr - label_ptr > 0xFFFF)
			ERROR("too large jump for branch instruction");

		value = (uint16_t) (label_ptr - current_ptr);

		// This assumes that labels will only be used with SIMM16 operands
		microcode.code[occurrence.instr_ptr] |= (0xFFFF & value);
	}
}