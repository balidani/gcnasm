#include <stdio.h>

#include "CuTest.h"

CuSuite* VOP2_GetSuite();

void run_all_tests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//add tests suits below
	CuSuiteAddSuite(suite, VOP2_GetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	run_all_tests();
}
