/*
 * AMD GCN ISA Assembler
 *
 * MTBUF instruction parser
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 */

#include <string.h>
#include <ctype.h>

#include "mtbuf.h"

/**
 * Parses instructions with a MTBUF encoding
 * 
 * MAGIC (6) | NFMT(3) | DFMT (4) | OP (3) | ADDR64 | GLC 
 * | IDXEN | OFFEN | OFFSET (12) | SOFFSET(8) | TFE
 * | SLC | r | SRSRC (5) | VDATA (8) | VADDR (8)
 */
isa_op_code* parseMTBUF(isa_instr instr, int argc, char **args)
{
	char *vdata_str, *vaddr_str, *srsrc_str, *soffset_str;
	int offset_parsed;
	int i, j;

	isa_operand *vdata_op, *vaddr_op, 	// ISA operand structs
		*srsrc_op, *soffset_op;	
	isa_op_code *op_code;				// Generated opcode struct

	char *format_token = "FORMAT:[";
	
	op_code = (isa_op_code *) malloc(sizeof(isa_op_code));
	
	if (argc < 4)
		ERROR("number of passed operands is too low");

	// Setup arguments
	vdata_str	= args[0];
	vaddr_str	= args[1];
	srsrc_str	= args[2];
	soffset_str	= args[3];

	// Parse operands
	op_code->code = instr.op_code;
	op_code->literal = 0;
	op_code->literal_set = 1;

	// VDATA
	vdata_op = parseOperand(vdata_str);

	if (vdata_op->op_type.type != VGPR)
		ERROR("VDATA operand must be a VGPR");

	op_code->literal |= vdata_op->op_code << 8;

	// VADDR
	vaddr_op = parseOperand(vaddr_str);

	if (vaddr_op->op_type.type != VGPR)
		ERROR("VADDR operand must be a VGPR");

	op_code->literal |= vaddr_op->op_code;

	// SRSRC
	srsrc_op = parseOperand(srsrc_str);

	// This field is missing 2 bits from LSB
	srsrc_op->op_code >>= 2;

	if (srsrc_op->op_type.type != SGPR)
		ERROR("SRSRC operand must be an SGPR");

	op_code->literal |= srsrc_op->op_code << 16;

	// SOFFSET
	soffset_op = parseOperand(soffset_str);

	if (soffset_op->op_type.type != SGPR
			&& soffset_op->op_type.type != M0
			&& soffset_op->op_type.type != ZERO
			&& soffset_op->op_type.type != INL_POS
			&& soffset_op->op_type.type != INL_NEG)
		ERROR("SOFFSET operand must be an SGPR, M0 or inline constant");

	op_code->literal |= soffset_op->op_code << 24;

	// Parse optional parameters
	offset_parsed = 0;

	for (i = 4; i < argc; ++i)
	{
		// Convert to upper case
		for (j = 0; args[i][j]; ++j)
			args[i][j] = (char) toupper(args[i][j]);

		if (strcmp(args[i], "OFFEN") == 0)
			op_code->code |= (1 << 12);
		else if (strcmp(args[i], "IDXEN") == 0)
			op_code->code |= (1 << 13);
		else if (strcmp(args[i], "GLC") == 0)
			op_code->code |= (1 << 14);
		else if (strcmp(args[i], "ADDR64") == 0)
			op_code->code |= (1 << 15);
		else if (strcmp(args[i], "SLC") == 0)
			op_code->literal |= (1 << 22);
		else if (strcmp(args[i], "TFE") == 0)
			op_code->literal |= (1 << 23);
		else if (strncmp(args[i], format_token, strlen(format_token)) == 0)
		{
			char *dfmt_token = "BUF_DATA_FORMAT_";
			char *nfmt_token = "BUF_NUM_FORMAT_";
			
			char *dfmt_str, *nfmt_str;
			
			char *dfmt_values[] = {
				"8", "16", "8_8", "32", "16_16", "10_11_11", "11_11_10",
				"10_10_10_2", "2_10_10_10", "8_8_8_8", "32_32", "16_16_16_16",
				"32_32_32", "32_32_32_32"
			};
			int dfmt_count = sizeof(dfmt_values) / sizeof(char *);

			char *nfmt_values[] = {
				"UNORM", "SNORM", "USCALED", "SSCALED", "UINT", "SINT", 
				"SNORM_OGL", "FLOAT"
			};
			int nfmt_count = sizeof(nfmt_values) / sizeof(char *);

			// Check tokens
			if (strncmp(args[i] + strlen(format_token), dfmt_token, 
					strlen(dfmt_token)) != 0)
				ERROR("invalid DFMT token format in MTBUF");

			if (strncmp(args[i+1], nfmt_token, 
					strlen(nfmt_token)) != 0)
				ERROR("invalid NFMT token format in MTBUF");

			dfmt_str = args[i] + strlen(format_token) + strlen(dfmt_token);
			nfmt_str = args[i+1] + strlen(nfmt_token);
			nfmt_str[strlen(nfmt_str) - 1] = '\0';

			// Find parameters
			for (j = 0; j < dfmt_count; ++j)
				if (strcmp(dfmt_str, dfmt_values[j]) == 0)
				{
					op_code->code |= ((uint32_t) j + 1) << 19;
					break;
				}

			for (j = 0; j < nfmt_count; ++j)
				if (strcmp(nfmt_str, nfmt_values[j]) == 0)
				{
					op_code->code |= ((uint32_t) j) << 23;
					break;
				}

			// Parser trick, because the format part is broken up here
			// due to the comma present in the parameters
			++i;
		}
		else if (!offset_parsed)
		{
			printf("X: %s\n", args[i]);
			isa_operand *offset_op = parseOperand(args[i]);

			if (!isConstantOperand(offset_op))
				ERROR("non-literal value supplied for OFFSET operand");

			op_code->code |= offset_op->value;
			offset_parsed = 1;
			
			free(offset_op);
		}

	}

	free(vdata_op);
	free(vaddr_op);
	free(srsrc_op);
	free(soffset_op);

	return op_code;
}