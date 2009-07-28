#include <stdlib.h>
#include <stdio.h>

#include "ctest.h"


void
test_segfault (CTestCase *tc, gpointer data)
{
	int *bad = NULL;
	*bad = 7;
}


void
test_division_by_zero (CTestCase *tc, gpointer data)
{
	int x = 8;
	int y = 0;

	int z = x/y;

	printf ("%d", z);
}



int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case (test, test_segfault);
	ctest_add_case (test, test_division_by_zero);

	ctest_run (test);

	//ctest_free (test);

	return ctest_finish (test);
}
