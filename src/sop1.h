/*
 * AMD GCN ISA Assembler
 *
 * SOP1 instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _SOP1_H
#define _SOP1_H

#include "isa_instr.h"
#include "isa_operand.h"
#include "error.h"

isa_op_code* parseSOP1(isa_instr instr, int argc, char **args);

#endif