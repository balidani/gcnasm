/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA structures
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

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

typedef enum 
{
	SGPR,
	/*VCC_LO, 
	VCC_HI,
	TBA_LO,
	TBA_HI,
	TMA_LO,
	TMA_HI,
	TTMP,
	M0,
	EXEC_LO,
	EXEC_HI,*/
	CONST_POS,
	CONST_NEG,
	LITERAL,
	ERROR
} isa_operand_type;

typedef struct 
{
	const char *name;
	uint32_t op_code;
	isa_instr_enc encoding;
} isa_instr;

typedef struct
{
	uint8_t op_code;
	uint32_t value;
	isa_operand_type type;
} isa_operand;

extern isa_instr isa_instr_list[];
extern int isa_instr_count;
