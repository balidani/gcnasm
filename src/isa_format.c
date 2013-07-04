/*
 * AMD GCN ISA Assembler
 *
 * GCN ISA instruction formats
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "isa_format.h"

const isa_format isa_format_list[] = 
{
	{SOP2,	3},
	{SOPK,	2},
	{SOP1,	2},
	{SOPC,	2},
	{SOPP,	1},
	{SMRD,	3},
	{VOP2,	3},
	{VOP1,	2},
	{VOPC,	2},
	{MUBUF, 12},
	{MTBUF, 13},
};

const int isa_format_count = sizeof(isa_format_list) / sizeof(isa_format);