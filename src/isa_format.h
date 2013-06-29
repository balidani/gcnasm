/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA instruction formats
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _ISA_FORMAT_H
#define _ISA_FORMAT_H

#include <stdint.h>

#include "isa_instr.h"

typedef struct 
{
	isa_instr_enc encoding;	// Instruction format encoding
	uint8_t op_count;		// Number of operands
} isa_format;

extern const isa_format isa_format_list[];
extern const int isa_format_count;

#endif