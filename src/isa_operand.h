/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA operand structures
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
	// Scalar general-purpose registers
	SGPR,
	// VCC
	VCC_LO, VCC_HI,
	// Trap handler base address
	TBA_LO, TBA_HI,
	// Pointer to data in memory used by trap handler
	TMA_LO,	TMA_HI,
	// Ttrap handler temporary registers
	TTMP,
	// Memory register 0
	M0,
	// EXEC
	EXEC_LO, EXEC_HI,
	// Zero
	ZERO,
	// Positive inline constant 1 - 64
	INL_POS,
	// Negative inline constant -1 - -16 
	INL_NEG,
	// Double precision inline constants (TODO)
	INL_HLF, INL_HLF_N,
	INL_ONE, INL_ONE_N,
	INL_TWO, INL_TWO_N,
	INL_FOUR, INL_FOUR_N,
	// VCCZ
	VCCZ,
	// EXECZ
	EXECZ,
	// SCC
	SCC,
	// Literal constant
	LITERAL,
	ERROR
} isa_operand_type;

typedef struct
{
	uint8_t op_code;		// Generated Opcode (stub)
	uint32_t value;			// Literal constant value
	isa_operand_type type;	// Operand type
} isa_operand;

// This won't be used for now
/*
typedef struct
{
	const char *name;		// Name of the operand
	uint8_t op_code;		// Generated Opcode (stub)
	uint32_t literal_value;	// Literal constant value
	isa_operand_type type;	// Operand type
	int is_mapped;			// Operand type should be mapped (e.g. SGPRs)
} isa_operand;

extern const isa_operand isa_operand_list[];
extern const int isa_operand_count;
*/