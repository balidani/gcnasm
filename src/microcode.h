/*
 * AMD GCN ISA Assembler
 *
 * Microcode module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _MICRO_H
#define _MICRO_H

#include <stdint.h>

typedef struct {
	uint32_t *code;
	uint32_t ptr;
} microcode_struct;

extern microcode_struct microcode;

#endif