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

#include "microcode.h"
#include "isa_instr.h"
#include "isa_operand.h"
#include "isa_format.h"
#include "error.h"
#include "alias.h"
#include "label.h"

#include "sop2.h"
#include "sopk.h"
#include "sop1.h"
#include "sopc.h"
#include "sopp.h"
#include "smrd.h"
#include "vop2.h"
#include "vop1.h"
#include "vopc.h"
#include "vop3a.h"
#include "vop3b.h"
#include "ds.h"
#include "mubuf.h"
#include "mtbuf.h"
#include "mimg.h"

typedef struct {
	isa_instr_enc encoding;
	isa_op_code* (* parser)();
} format_parser;

void parseFile(const char *input, const char *output);
isa_op_code* parseLine(char *line);
char* parseField(char **line, const char delimiter[]);

#endif