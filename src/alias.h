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

#ifndef _ALIAS_H
#define _ALIAS_H

#include "error.h"

typedef struct {
	char *tag;		// passed string, e.g. "exec"
	char *operand;	// real operand, e.g. "EXEC_LO"
} alias;

void initAlias();
void clearAlias();
void addAlias(char *tag, char *operand);

extern alias *alias_list;
extern unsigned int alias_count;

#endif