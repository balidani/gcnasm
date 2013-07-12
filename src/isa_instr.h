/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA instruction structures
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _ISA_INSTR_H
#define _ISA_INSTR_H

#include <stdint.h>

typedef enum 
{
	NONE,
	SOP1, 
	SOP2, 
	SOPK, 
	SOPC, 
	SOPP, 
	SMRD,
	VOP1, 
	VOP2, 
	VOP3a,
	VOP3b, 
	VOPC, 
	VINTRP,
	DS, 
	EXP, 
	MUBUF, 
	MTBUF, 
	MIMG
} isa_instr_enc;

typedef struct 
{
	// Name of the instruction
	const char *name;
	// 32 bit opcode value
	uint32_t op_code;
	// Opcode encoding type
	isa_instr_enc encoding;
	// Alternate encoding type
	isa_instr_enc alt_encoding;
	// Alternate opcode
	uint32_t alt_op_code;
} isa_instr;

extern const isa_instr isa_instr_list[];
extern const unsigned int isa_instr_count;

#endif