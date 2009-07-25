#ifndef __CTEST_H__
#define __CTEST_H__

#include <glib.h>

typedef struct CTest     CTest;
typedef struct CTestCase CTestCase;

typedef void (*CTestFunction) (CTest *test, gpointer data);
typedef void (*CTestCaseFunction) (CTestCase *tc, gpointer *data);

struct CTest {
	CTestFunction  set_up;
	CTestFunction  tear_down;
	gpointer       test_data;

	gint           cases_total;
	gint           cases_ok;

	GList         *test_cases;
};

struct CTestCase {
	CTest             *test;
	CTestCaseFunction  func;
	gint               asserts_total;
	gint               asserts_ok;
};


/* CTest api */

CTest *
ctest_new (gpointer test_data);

void
ctest_free (CTest *test);

void
ctest_add_case (CTest *test, CTestCaseFunction func);

gboolean
ctest_run (CTest *test);


CTestCase *
ctest_case_new (CTestCaseFunction func);

void
ctest_case_free (CTestCase *tc);


/* Test case API */

void ctest_case_assert              (CTestCase *tc, gint condition, const gchar *file, gint line, const gchar *msg, ...);
void ctest_case_assert_true         (CTestCase *tc, gint condition, const gchar *file, gint line);
void ctest_case_assert_equal_int    (CTestCase *tc, gint i1, gint i2, const gchar *file, gint line);
void ctest_case_assert_equal_long   (CTestCase *tc, glong l1, glong l2, const gchar *file, gint line);
void ctest_case_assert_equal_string (CTestCase *tc, const gchar *s1, const gchar *s2, const gchar *file, gint line);
void ctest_case_assert_not_reached  (CTestCase *tc, const gchar *file, gint line);

/* Test case API macros */

#define ctest_assert(tc, condition, ...)\
	ctest_case_assert(tc, condition, __FILE__, __LINE__, args...);

#define ctest_assert_true(tc, condition)\
	ctest_case_assert_true(tc, condition, __FILE__, __LINE__);

#define ctest_assert_equal(tc, expected, got, fmt)\
	ctest_case_assert(tc, (x1)==(x2), __FILE__, __LINE__, "expected %" fmt " got %" fmt, expected, got);

#define ctest_assert_equal_string(tc, expected, got)\
	ctest_case_assert_equal_string(tc, expected, got, __FILE__, __LINE__, args...);

#define ctest_assert_not_reached(tc)\
	ctest_case_assert_not_reached(tc, __FILE__, __LINE__);


#endif /* __CTEST_H__ */
