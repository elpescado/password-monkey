#include <stdio.h>
#include <stdarg.h>

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
ctest_add_case_full (CTest *test, const gchar *name, CTestCaseFunction func)
{
	CTestCase *tc = ctest_case_new (func);
	tc->test = test;
	tc->name = g_strdup (name);

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


/*** ASSERTIONS ***/


void ctest_case_assert              (CTestCase *tc, gint condition, const gchar *file, gint line, const gchar *msg, ...)
{
	tc->asserts_total++;
	if (condition) {
		tc->asserts_ok++;
	} else {
		fprintf (stderr, " *** %s:%d: ", file, line);

		va_list ap;
		va_start (ap, msg);
		vfprintf (stderr, msg, ap);
		va_end (ap);

		fprintf (stderr, "\n");
	}
}

/*
void ctest_case_assert_true         (CTestCase *tc, gint condition, const gchar *file, gint line)
{
	ctest_case_assert (tc, condition, file, line, "Assertion failed");
}
*/

void ctest_case_assert_equal_int    (CTestCase *tc, gint i1, gint i2, const gchar *file, gint line)
{
}


void ctest_case_assert_equal_long   (CTestCase *tc, glong l1, glong l2, const gchar *file, gint line)
{
}


void ctest_case_assert_equal_string (CTestCase *tc, const gchar *s1, const gchar *s2, const gchar *file, gint line)
{
}


void ctest_case_assert_not_reached  (CTestCase *tc, const gchar *file, gint line)
{
}




