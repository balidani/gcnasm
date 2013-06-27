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

#include "parser.h"

int main(int argc, char **argv) 
{

	if (argc != 3) 
	{

		printf("Usage: %s <input> <output>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	parseFile("input.isa", "output.bin");

	exit(EXIT_SUCCESS);
}