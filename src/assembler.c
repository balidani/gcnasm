/*
 * AMD GCN ISA Assembler
 *
 * Main module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "alias.h"
#include "parser.h"

int main(int argc, char **argv) 
{
	if (argc != 3) 
	{
		printf("Usage: %s <input> <output>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Initialize alias list
	initAlias();

	// Parse the input ISA file and write to output
	parseFile(argv[1], argv[2]);

	// Free alias list
	clearAlias();

	exit(EXIT_SUCCESS);
}