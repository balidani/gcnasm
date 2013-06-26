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

#define SDST_OPERAND_TRESHOLD 128

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
	SGPR		= 0,		// Scalar general-purpose register
	VCC_LO		= 106, 
	VCC_HI,
	TBA_LO,
	TBA_HI,
	TMA_LO,
	TMA_HI,
	TTMP,
	M0			= 124,
	EXEC_LO		= 126,
	EXEC_HI,
	CONST_POS,				// Positive constant 0 - 64
	CONST_NEG	= 193,		// Negative constant -1 - -16
	CONST_HLF	= 240,		// 0.5
	CONST_HLF_N,			// -0.5
	CONST_ONE,				// 1.0
	CONST_ONE_N,			// -1.0
	CONST_TWO,				// 2.0
	CONST_TWO_N,			// -2.0
	CONST_FOUR,				// 4.0
	CONST_FOUR_N,			// -4.0
	VCCZ		= 251,
	EXECZ,
	SCC,
	LITERAL		= 255,		// Large constant, literal value follows
	ERROR					// Unknown operand type - error
} isa_operand_type;

typedef struct 
{
	const char *name;		// Name of the instruction
	uint32_t op_code;		// 32 bit opcode value
	isa_instr_enc encoding;	// Opcode encoding type
} isa_instr;

typedef struct
{
	uint8_t op_code;		// Generated Opcode
	uint32_t value;			// Operand value (for literal values)
	isa_operand_type type;	// Operand type
} isa_operand;

typedef struct {
	uint32_t code;			// 32 bit opcode value
	uint32_t literal;		// Optional 32 bit literal value
	int literal_set;		// Flag to note the presence of a literal value
} isa_op_code;

extern const isa_instr isa_instr_list[];
extern const int isa_instr_count;
