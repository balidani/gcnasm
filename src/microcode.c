/*
 * AMD GCN ISA Assembler
 *
 * Microcode module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include "microcode.h"

// Global microcode struct to keep track of EIP
microcode_struct microcode;