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

#ifndef _ERROR_H
#define _ERROR_H

#define ERROR(...)	{					\
	fflush(stdout);						\
	fprintf(stderr, "\n[*] Error: ");	\
	fprintf(stderr, __VA_ARGS__);		\
	fprintf(stderr, "\n");				\
	exit(EXIT_FAILURE);					\
}

#define WARNING(...) {					\
	fflush(stdout);						\
	fprintf(stderr, "\n[*] Warning: ");	\
	fprintf(stderr, __VA_ARGS__);		\
	fprintf(stderr, "\n");				\
}

#endif