#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ctest.h"


CTest *
ctest_new (gpointer test_data)
{
	CTest *test = g_new0 (CTest, 1);

	test->test_data = test_data;
	test->fork_tests = TRUE;
	
	return test;
}


void
ctest_free (CTest *test)
{
	g_list_free (test->test_cases);
	g_free (test);
}


CTestCase *
ctest_add_case_full (CTest *test, const gchar *name, CTestCaseFunction func)
{
	CTestCase *tc = ctest_case_new (func);
	tc->test = test;
	tc->name = g_strdup (name);

	test->test_cases = g_list_append (test->test_cases, tc);

	return tc;
}


CTestCase *
ctest_add_case_full_dataset (CTest             *test,
                             const gchar       *name,
                             CTestCaseFunction  func,
                             const void        *data,
                             size_t             element_size,
                             size_t             n_elements)
{
	CTestCase *tc;

	tc = ctest_add_case_full (test, name, func);

	/*
	g_printerr ("Data: %p\n", data);
	g_printerr ("Element size: %d\n", element_size);
	g_printerr ("Number of elements: %d\n", n_elements);
	*/

	tc->dataset = data;
	tc->dataset_size = n_elements;
	tc->dataset_item_size = element_size;

	return tc;
}


const void *
ctest_case_get_data (CTestCase *tc)
{
	if (tc->dataset)
		return tc->dataset + tc->dataset_iter*tc->dataset_item_size;	
	else
		return NULL;
}


static gboolean
ctest_run_test_case_real (CTest *test, CTestCase *tc)
{
	tc->func (tc, test->test_data);

	g_printerr (" -> %d of %d asserts ok\n", tc->asserts_ok, tc->asserts_total);
	return tc->asserts_total == tc->asserts_ok;
}


gboolean
ctest_run_single_test_case (CTest *test, CTestCase *tc)
{
	tc->asserts_total = tc->asserts_ok = 0;

	if (test->fork_tests) {
		pid_t pid = fork ();
		if (pid < 0) {
			/* Error */
		} else if (pid == 0) {
			/* Child */

			ctest_run_test_case_real (test, tc);
			//tc->func (tc, test->test_data);

			if (tc->asserts_total == tc->asserts_ok)
				exit (EXIT_SUCCESS);
			else
				exit (EXIT_FAILURE);

		} else {
			/* Parent */

			int status;
			waitpid (pid, &status, 0);

			if (WIFEXITED (status)) {
				/* Test exited normally */

				int exit_code = WEXITSTATUS (status);

				if (exit_code == EXIT_SUCCESS)
					return TRUE;

			} else if (WIFSIGNALED (status)) {
				/* Test caught signal */

				g_printerr ("ERROR: Test interrupted by signal %d %s\n",
						WTERMSIG (status),
						WCOREDUMP (status) ? "(core dumped)" : "");

				return FALSE;

			} else {
				/* Error */

				g_printerr ("ERROR: generic error\n");

				return FALSE;
			}
		}
	} else {
		return ctest_run_test_case_real (test, tc);
		//tc->func (tc, test->test_data);
	}
	return FALSE;
}

gboolean
ctest_run_test_case (CTest *test, CTestCase *tc)
{
	if (tc->dataset) {
		gboolean ok = TRUE;
		for (tc->dataset_iter = 0; tc->dataset_iter < tc->dataset_size; tc->dataset_iter++) {
			ok = ok && ctest_run_single_test_case (test, tc);
		}
		return ok;
	} else {
		return ctest_run_single_test_case (test, tc);
	}
}

gboolean
ctest_run (CTest *test)
{
	GList *iter = test->test_cases;
	gboolean all_ok = TRUE;

	test->cases_total = test->cases_ok = 0;

	while (iter) {
		CTestCase *tc = iter->data;

		test->cases_total++;

		g_printerr ("\n%s:\n", tc->name);
	
		if (!ctest_run_test_case (test, tc)) {
			all_ok = FALSE;
		} else {
			test->cases_ok++;
		}

		iter = iter->next;
	}

	return all_ok;
}


int
ctest_finish (CTest *test)
{
	int ok = test->cases_total == test->cases_ok;

	g_printerr ("\nTest summary:\n");
	g_printerr ("%d of %d test cases passed\n", test->cases_ok, test->cases_total);

	ctest_free (test);

	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
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
	gboolean ok = FALSE;
	if (s1 == NULL && s2 == NULL)
		ok = TRUE;
	else if (s1 != NULL && s2 != NULL && strcmp (s1, s2) == 0)
		ok = TRUE;

	ctest_case_assert (tc, ok, file, line, "Expected '%s' got '%s'\n", s1, s2);
}


void ctest_case_assert_not_reached  (CTestCase *tc, const gchar *file, gint line)
{
	ctest_case_assert (tc, 0, file, line, "Assertion reached");
}




