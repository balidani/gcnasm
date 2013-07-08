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
	TEST_isa_op_code("s_mul_i32     s0, s12, s0",0x9300000C,0x00000000,0);
	TEST_isa_op_code("s_mul_i32     s11, s11, s4",0x930B040B,0x00000000,0);
}

void S_LSHL_B32_test(CuTest* tc)
{
	TEST_isa_op_code("s_lshl_b32    s2, s4, 2",0x8F028204,0x00000000,0);
	TEST_isa_op_code("s_lshl_b32    s0, s4, 1",0x8F008104,0x00000000,0);

	TEST_isa_op_code("s_lshl_b32    s0, s4, 3",0x8F008304,0x00000000,0);
	TEST_isa_op_code("s_lshl_b32    s1, s4, 2",0x8F018204,0x00000000,0);
	TEST_isa_op_code("s_lshl_b32    s9, s7, 3",0x8F098307,0x00000000,0);
	TEST_isa_op_code("s_lshr_b32    s11, s6, 2 ",0x900B8206,0x00000000,0);
}

void S_ANDN2_B64_test(CuTest* tc)
{
	TEST_isa_op_code("s_andn2_b64   s[28:29], s[28:29], s[30:31]",0x8A9C1E1C,0x00000000,0);
//	uncomment if aliases are supported
//	TEST_isa_op_code("s_andn2_b64   exec, s[8:9], s[6:7]",0x8AFE0608,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[6:7], s[0:1]",0x8AFE0006,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[30:31], s[32:33]",0x8AFE201E,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   s[10:11], s[10:11], exec",0x8A8A7E0A,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[6:7], exec",0x8AFE7E06,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   s[14:15], s[14:15], exec",0x8A8E7E0E,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[6:7], exec",0x8AFE7E06,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[10:11], s[0:1]",0x8AFE000A,0x00000000,0);
//	TEST_isa_op_code("s_andn2_b64   exec, s[20:21], exec",0x8AFE7E14,0x00000000,0);
//	TEST_isa_op_code("s_and_b64     exec, s[20:21], s[14:15]",0x87FE0E14,0x00000000,0);
}

void S_AND_B64_test(CuTest* tc)
{
//	uncomment if aliases are supported
//	TEST_isa_op_code("s_and_b64     exec, s[24:25], s[14:15]",0x87FE0E18,0x00000000,0);
//	TEST_isa_op_code("s_and_b64     exec, s[20:21], s[14:15]",0x87FE0E14,0x00000000,0);
//	TEST_isa_op_code("s_and_b64     exec, s[6:7], s[0:1]",0x87FE0006,0x00000000,0);
//	TEST_isa_op_code("s_and_b64     exec, s[22:23], s[20:21]",0x87FE1416,0x00000000,0);
}

void S_AND_B32_test(CuTest* tc)
{
	TEST_isa_op_code("s_and_b32     s9, s9, 56",0x8709B809,0x00000000,0);
	TEST_isa_op_code("s_and_b32     s10, -8, s7",0x870A07C8,0x00000000,0);
	TEST_isa_op_code("s_and_b32     s10, s5, 24",0x870A9805,0x00000000,0);
}

void S_ADD_I32_test(CuTest* tc)
{
	TEST_isa_op_code("s_add_i32     s0, s0, s1",0x81000100,0x00000000,0);
	TEST_isa_op_code("s_add_i32     s11, s6, 1",0x810B8106,0x00000000,0);
}

void S_ADD_U32_test(CuTest* tc)
{
	TEST_isa_op_code("s_add_u32     s0, s0, 0x0000a38c",0x8000FF00,0x0000A38C,1);
	TEST_isa_op_code("s_add_u32     s14, s14, 0x0000a1c4",0x800EFF0E,0x0000A1C4,1);
	TEST_isa_op_code("s_add_u32     s10, s10, 0x0000a104",0x800AFF0A,0x0000A104,1);
}

void S_ADDC_U32_test(CuTest* tc)
{
	TEST_isa_op_code("s_addc_u32    s15, s15, 0",0x820F800F,0x00000000,0);
	TEST_isa_op_code("s_addc_u32    s1, s1, 0",0x82018001,0x00000000,0);
	TEST_isa_op_code("s_addc_u32    s11, s11, 0",0x820B800B,0x00000000,0);
}

void S_SUB_U32_test(CuTest* tc)
{
	TEST_isa_op_code("s_sub_u32     s14, s14, 0x0000a354",0x808EFF0E,0x0000A354,1);
	TEST_isa_op_code("s_sub_u32     s6, s6, 0x0000a1f4",0x8086FF06,0x0000A1F4,1);
}

void S_SUBB_U32_test(CuTest* tc)
{
	TEST_isa_op_code("s_subb_u32    s15, s15, 0",0x828F800F,0x00000000,0);
	TEST_isa_op_code("s_subb_u32    s7, s7, 0",0x82878007,0x00000000,0);
}

CuSuite* VOP2_GetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, S_MIN_U32_test);
	SUITE_ADD_TEST(suite, S_MUL_I32_test);
	SUITE_ADD_TEST(suite, S_LSHL_B32_test);
	SUITE_ADD_TEST(suite, S_ANDN2_B64_test);
	SUITE_ADD_TEST(suite, S_ADD_I32_test);
	SUITE_ADD_TEST(suite, S_ADD_U32_test);
	SUITE_ADD_TEST(suite, S_ADDC_U32_test);
	SUITE_ADD_TEST(suite, S_AND_B64_test);
	SUITE_ADD_TEST(suite, S_AND_B32_test);
	SUITE_ADD_TEST(suite, S_SUB_U32_test);
	SUITE_ADD_TEST(suite, S_SUBB_U32_test);

	return suite;
}

