#include "ctest.h"


CTest *
ctest_new (gpointer test_data)
{
	CTest *test = g_new0 (CTest, 1);

	test->test_data = test_data;
	
	return test;
}


void
ctest_free (CTest *test)
{
	g_list_free (test->test_cases);
	g_free (test);
}


void
ctest_add_case (CTest *test, CTestCaseFunction func)
{
	CTestCase *tc = ctest_case_new (func);
	tc->test = test;

	test->test_cases = g_list_append (test->test_cases, tc);
}


gboolean
ctest_run_test_case (CTest *test, CTestCase *tc)
{
	tc->func (tc, test->test_data);
	return TRUE;
}


gboolean
ctest_run (CTest *test)
{
	GList *iter = test->test_cases;
	gboolean all_ok = TRUE;

	while (iter) {
		CTestCase *tc = iter->data;
	
		if (!ctest_run_test_case (test, tc)) {
			all_ok = FALSE;
		}

		iter = iter->next;
	}

	return all_ok;
}


CTestCase *
ctest_case_new (CTestCaseFunction func)
{
	CTestCase *tc = g_new0 (CTestCase, 1);

	tc->func = func;

	return tc;
}


void
ctest_case_free (CTestCase *tc)
{
	g_free (tc);
}

