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
	XOP,
	SOP1, 
	SOP2, 
	SOPK, 
	SOPC, 
	SOPP, 
	SMRD,
	VOP1, 
	VOP2, 
	VOP3, 
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
	const char *name;		// Name of the instruction
	uint32_t op_code;		// 32 bit opcode value
	isa_instr_enc encoding;	// Opcode encoding type
} isa_instr;

extern const isa_instr isa_instr_list[];
extern const int isa_instr_count;

#endif