/*
 * AMD GCN ISA Assembler
 *
 * VOP2 instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _VOP2_H
#define _VOP2_H

#include "isa_instr.h"
#include "isa_operand.h"
#include "error.h"

isa_op_code* parseVOP2(isa_instr instr, char **args);

#endif