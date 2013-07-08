#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../isa_operand.h"
#include "../parser.h"
/*-------------------------------------------------------------------------*
 * VOP2 Test
 *-------------------------------------------------------------------------*/


int assert_isa_op_code_equal(CuTest* tc, isa_op_code *result, isa_op_code *expected)
{
	int ret=1;
	if(result->code != expected->code) ret=0;
	if(result->literal != expected->literal) ret=0;
	if(result->literal_set != expected->literal_set) ret=0;

	if(!ret)
	{
		printf("code = %08x, expected = %08x\n",result->code,expected->code);
		printf("literal = %08x, expected = %08x\n",result->literal,expected->literal);
		printf("literal_set = %d, expected = %d\n",result->literal_set,expected->literal_set);
	}
	return ret;
}

#define TEST_isa_op_code(line,code,literal,literal_set)\
	{char sline[]=line;\
	isa_op_code expected={code,literal,literal_set};\
	isa_op_code *ret=parseLine(sline);\
	CuAssertTrue(tc,assert_isa_op_code_equal(tc,ret,&expected));\
	free(ret);}
	

void S_MIN_U32_test(CuTest* tc)
{
	//s_min_u32     s0, s0, 0x0000ffff                          // 0000000C: 8380FF00 0000FFFF
	TEST_isa_op_code("s_min_u32     s0, s0, 0x0000ffff",0x8380ff00,0x0000ffff,1);
}

void S_MUL_I32_test(CuTest* tc)
{
	//s_mul_i32     s0, s4, 3                                   // 00000B80: 93008304
	TEST_isa_op_code("s_mul_i32     s0, s4, 3",0x93008304,0x00000000,0);
}

void S_LSHL_B32_test(CuTest* tc)
{
	TEST_isa_op_code("s_lshl_b32    s2, s4, 2",0x8F028204,0x00000000,0);
	TEST_isa_op_code("s_lshl_b32    s0, s4, 1",0x8F008104,0x00000000,0);
}

void S_ANDN2_B64_test(CuTest* tc)
{
	TEST_isa_op_code("s_andn2_b64   s[28:29], s[28:29], s[30:31]",0x8A9C1E1C,0x00000000,0);
	TEST_isa_op_code("s_andn2_b64   exec, s[8:9], s[6:7]",0x8AFE0608,0x00000000,0);
}




CuSuite* VOP2_GetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, S_MIN_U32_test);
	SUITE_ADD_TEST(suite, S_MUL_I32_test);
	SUITE_ADD_TEST(suite, S_LSHL_B32_test);
	SUITE_ADD_TEST(suite, S_ANDN2_B64_test);

	return suite;
}

