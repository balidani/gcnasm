/*
 * AMD GCN ISA Assembler
 *
 * Alias handling
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "alias.h"

alias *alias_list;
unsigned int alias_count;

// A reasonal upper limit for aliases
// TODO: in later versions an unlimited number 
// of aliases should be supported
static const unsigned int max_alias = 256;

/**
 * Alias list initialization
 */
void initAlias()
{
	alias_list = (alias *) calloc((size_t) max_alias, sizeof(alias));
	if(!alias_list)
		ERROR("cannot allocate memory for alias_list");
	alias_count = 0;

	// Initial aliases are added here
	addAlias("EXEC", "EXEC_LO");
	addAlias("VCC", "VCC_LO");
	addAlias("TBA", "TBA_LO");
	addAlias("TMA", "TMA_LO");
	addAlias("LGKMCNT(0)", "0x07F");
	addAlias("VMCNT(0)", "0xF70");
	addAlias("EXPCNT(0)", "0xF0F");
}

/**
 * Frees the alias list from the memory
 */
void clearAlias()
{
	unsigned int i;

	for (i = 0; i < alias_count; ++i)
	{
		free(alias_list[i].tag);
		free(alias_list[i].operand);
	}

	free(alias_list);
}

/**
 * Adds a new alias to the list
 */
void addAlias(char *tag, char *operand)
{
	int i;

	if (alias_count == max_alias)
		ERROR("maximum alias count reached");

	char *tag_copy, *operand_copy;

	tag_copy = (char *) calloc(strlen(tag) + 1, sizeof(char));
	operand_copy = (char *) calloc(strlen(operand) + 1, sizeof(char));

	strncpy(tag_copy, tag, strlen(tag));
	strncpy(operand_copy, operand, strlen(operand));

	// Convert alias to upper-case
	for (i = 0; tag_copy[i]; ++i)
		tag_copy[i] = (char) toupper(tag_copy[i]);

	for (i = 0; operand_copy[i]; ++i)
		operand_copy[i] = (char) toupper(operand_copy[i]);

	alias_list[alias_count].tag = tag_copy;
	alias_list[alias_count].operand = operand_copy;
	alias_count++;
}
