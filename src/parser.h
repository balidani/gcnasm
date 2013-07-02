/*
 * AMD GCN ISA Assembler
 *
 * Parser module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _PARSER_H
#define _PARSER_H

#include <stdint.h>

#include "isa_instr.h"
#include "isa_operand.h"
#include "isa_format.h"
#include "error.h"

#include "sop2.h"
#include "sopk.h"
#include "sop1.h"
#include "sopc.h"
#include "sopp.h"
#include "smrd.h"
#include "vop2.h"
#include "vop1.h"
#include "vopc.h"

void parseFile(const char *input, const char *output);

isa_op_code* parseLine(char *line);

void parseToken(char **line, char **res);

#endif