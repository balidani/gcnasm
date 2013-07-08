#include <stdio.h>

#include "CuTest.h"

CuSuite* SOP2_GetSuite();

void run_all_tests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//add tests suits below
	CuSuiteAddSuite(suite, SOP2_GetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	run_all_tests();
}
