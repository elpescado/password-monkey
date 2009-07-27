#include <stdlib.h>

#include "ctest.h"


void
test_addition (CTestCase *ct, gpointer *data)
{
	ctest_assert_true (ct, 1 + 1 == 2);
	ctest_assert_true (ct, 2 + 2 == 4);
	ctest_assert_true (ct, 1 + 0 == 1);
	ctest_assert_true (ct, 0 + 1 == 7);
}


void
test_subtraction (CTestCase *ct, gpointer *data)
{
	ctest_assert_equal (ct, 1 - 0,  1);
	ctest_assert_equal (ct, 0 - 1, -1);
}


int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case (test, test_addition);
	ctest_add_case (test, test_subtraction);

	ctest_run (test);

	ctest_free (test);

	return EXIT_SUCCESS;
}
