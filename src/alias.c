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
}

/**
 * Frees the alias list from the memory
 */
void clearAlias()
{
	free(alias_list);
}

/**
 * Adds a new alias to the list
 */
void addAlias(const char *tag, const char *operand)
{
	if (alias_count == max_alias)
		ERROR("maximum alias count reached");

	alias_list[alias_count].tag = tag;
	alias_list[alias_count].operand = operand;
	alias_count++;
}
