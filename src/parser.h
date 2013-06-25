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

 #include "isa_instr.h"

void parseFile(const char *input, const char *output);
void parseLine(char *line);

void parseSOP2(isa_instr instr, char *line);
isa_operand parseOperand(char *dst);