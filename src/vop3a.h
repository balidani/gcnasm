/*
 * AMD GCN ISA Assembler
 *
 * VOP3a instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _VOP3A_H
#define _VOP3A_H

#include "isa_instr.h"
#include "isa_operand.h"
#include "error.h"

isa_op_code* parseVOP3a(isa_instr instr, int argc, char **args);
char preProcessOp(char **op_str);

#endif