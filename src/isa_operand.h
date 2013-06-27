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
	ZERO = SDST_OPERAND_TRESHOLD,
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
} isa_operand_type_enum;

typedef struct
{
	const char *name;
	uint8_t op_code;
	isa_operand_type_enum type;
} isa_operand_type;

typedef struct
{
	uint8_t op_code;			// Generated Opcode (stub)
	uint32_t value;				// Literal constant value
	isa_operand_type op_type;	// Operand type
} isa_operand;


extern const isa_operand_type isa_simple_operand_list[];
extern const int isa_simple_operand_count;
extern const isa_operand_type isa_mapped_operand_list[];
extern const int isa_mapped_operand_count;

// Do we need this? Surely there is a better way. (TODO)
#define SGPR_OP 	(isa_mapped_operand_list[0])
#define TTMP_OP 	(isa_mapped_operand_list[1])
#define ZERO_OP 	(isa_mapped_operand_list[2])
#define INL_POS_OP 	(isa_mapped_operand_list[3])
#define INL_NEG_OP 	(isa_mapped_operand_list[4])
#define LITERAL_OP 	(isa_mapped_operand_list[5])
