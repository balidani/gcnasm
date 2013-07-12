/*
 * AMD GCN ISA Assembler
 *
 * Label handling
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#ifndef _LABEL_H
#define _LABEL_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "microcode.h"
#include "error.h"

typedef struct {
	char *name;
	uint32_t instr_ptr;
} label;

typedef struct {
	unsigned int label_id;
	uint32_t instr_ptr;
} label_occurrence;

void initLabel();
void clearLabel();

void addLabel(char *name, uint32_t instr_ptr);
void addOccurrence(unsigned int id, uint32_t ptr);

void parseLabel(char *line);
void processOccurrence();

extern label *label_list;
extern unsigned int label_count;

extern label_occurrence *occurrence_list;
extern unsigned int occurrence_count;

#endif