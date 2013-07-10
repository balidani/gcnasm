/*
 * AMD GCN ISA Assembler
 *
 * DS instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _DS_H
#define _DS_H

#include "isa_instr.h"
#include "isa_operand.h"
#include "error.h"

isa_op_code* parseDS(isa_instr instr, int argc, char **args);

#endif