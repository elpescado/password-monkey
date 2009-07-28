#include <stdlib.h>

#include "ctest.h"


void
test_addition (CTestCase *ct, gpointer data)
{
	ctest_assert_true (ct, 1 + 1 == 2);
	ctest_assert_true (ct, 2 + 2 == 4);
	ctest_assert_true (ct, 1 + 0 == 1);
	ctest_assert_true (ct, 0 + 1 == 1);
}


void
test_subtraction (CTestCase *ct, gpointer data)
{
	ctest_assert_equali (ct,  1, 1 - 0);
	ctest_assert_equali (ct, -1, 0 - 1);
}


void
test_floats (CTestCase *tc, gpointer data)
{
	float p = 1.0e-7;	/* Precision */

	ctest_assert_equalf (tc, 0.0f, 0.3f - 0.1f - 0.1f - 0.1f, p);
	ctest_assert_equalf (tc, 0.1f, 0.2f - 0.1f,               p);
	ctest_assert_equalf (tc, 0.3f, 0.1f + 0.1f + 0.1f,        p);
}



void
test_doubles (CTestCase *tc, gpointer data)
{
	double p = 1.0e-16;	/* Precision */

	ctest_assert_equalf (tc, 0.0, 0.3 - 0.1 - 0.1 - 0.1, p);
	ctest_assert_equalf (tc, 0.1, 0.2 - 0.1,             p);
	ctest_assert_equalf (tc, 0.3, 0.1 + 0.1 + 0.1,       p);
}



int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case (test, test_addition);
	ctest_add_case (test, test_subtraction);
	ctest_add_case (test, test_floats);
	ctest_add_case (test, test_doubles);

	ctest_run (test);

	//ctest_free (test);

	return ctest_finish (test);
}
