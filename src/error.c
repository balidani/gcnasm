/*
 * AMD GCN ISA Assembler
 *
 * Simple error handler
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void error(const char *msg)
{
	fflush(stdout);
	fprintf(stderr, "\n[*] Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void warning(const char *msg)
{
	fflush(stdout);
	fprintf(stderr, "\n[*] Warning: %s\n", msg);
}