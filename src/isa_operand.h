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

#ifndef _ISA_OPERAND_H
#define _ISA_OPERAND_H

#include <stdint.h>

#include "alias.h"
#include "error.h"

// These are still SOP2 dependent,
// and will probably be moved/changed

#define SDST_OPERAND_TRESHOLD 128

// Do we need this? Surely there is a better way. (TODO)
#define VGPR_OP 	(isa_mapped_operand_list[0])
#define SGPR_OP 	(isa_mapped_operand_list[1])
#define TTMP_OP 	(isa_mapped_operand_list[2])
#define ZERO_OP 	(isa_mapped_operand_list[3])
#define INL_POS_OP 	(isa_mapped_operand_list[4])
#define INL_NEG_OP 	(isa_mapped_operand_list[5])
#define LITERAL_OP 	(isa_mapped_operand_list[6])

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
	// SRC_LDS_DIRECT, special for VOP3
	SRC_LDS_DIR,
	// Literal constant
	LITERAL,
	ERROR,
	// Vector general-purpose registers
	VGPR
} isa_operand_type_enum;

typedef struct
{
	const char *name;
	uint32_t op_code;
	isa_operand_type_enum type;
} isa_operand_type;

typedef struct
{
	uint32_t op_code;			// Generated Opcode (stub)
	uint32_t value;				// Literal constant value
	isa_operand_type op_type;	// Operand type
} isa_operand;

typedef struct {
	uint32_t code;				// 32 bit opcode value
	uint32_t literal;			// Optional 32 bit literal value
	int literal_set;			// Flag noting the presence of a literal
} isa_op_code;

extern const isa_operand_type isa_simple_operand_list[];
extern const unsigned int isa_simple_operand_count;
extern const isa_operand_type isa_mapped_operand_list[];
extern const unsigned int isa_mapped_operand_count;

isa_operand* parseOperand(char *op_str);
void setLiteralOperand(isa_op_code *op_code, isa_operand *operand);
int isConstantOperand(isa_operand *operand);

#endif