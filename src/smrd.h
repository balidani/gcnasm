/*
 * AMD GCN ISA Assembler
 *
 * SMRD instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _SMRD_H
#define _SMRD_H

#include "isa_instr.h"
#include "isa_operand.h"
#include "error.h"

typedef enum {
	SDST,
	SBASE,
	OFFSET
} smrd_operand;

isa_op_code* parseSMRD(isa_instr instr, int argc, char **args);

#endif