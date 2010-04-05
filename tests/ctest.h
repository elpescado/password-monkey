#ifndef __CTEST_H__
#define __CTEST_H__

#include <glib.h>

typedef struct CTest     CTest;
typedef struct CTestCase CTestCase;

typedef void (*CTestFunction) (CTest *test, gpointer data);
typedef void (*CTestCaseFunction) (CTestCase *tc, gpointer data);


struct CTest {
	CTestFunction  set_up;
	CTestFunction  tear_down;
	gpointer       test_data;

	gint           cases_total;
	gint           cases_ok;

	gboolean       fork_tests;

	GList         *test_cases;
};

struct CTestCase {
	gchar             *name;
	CTest             *test;
	CTestCaseFunction  func;
	gint               asserts_total;
	gint               asserts_ok;

	/* Dataset */
	const void        *dataset;
	gint               dataset_size;
	gint               dataset_item_size;
	gint               dataset_iter;
};


/* CTest api */

CTest *
ctest_new (gpointer test_data);

void
ctest_free (CTest *test);

#define ctest_add_case(test, func) ctest_add_case_full (test, #func, func)

CTestCase *
ctest_add_case_full (CTest *test, const gchar *name, CTestCaseFunction func);

#define ctest_add_case_dataset(test, func, data) ctest_add_case_full_dataset (test, #func, func, data, sizeof(data[0]), sizeof(data)/sizeof(data[0]))

CTestCase *
ctest_add_case_full_dataset (CTest *test, const gchar *name, CTestCaseFunction func, const void *data, size_t element_size, size_t n_elements);

gboolean
ctest_run (CTest *test);

int
ctest_finish (CTest *tes);

CTestCase *
ctest_case_new (CTestCaseFunction func);

void
ctest_case_free (CTestCase *tc);

const void *
ctest_case_get_data (CTestCase *tc);

/* Test case core API */

void ctest_case_assert              (CTestCase *tc, gint condition, const gchar *file, gint line, const gchar *msg, ...);
//void ctest_case_assert_true         (CTestCase *tc, gint condition, const gchar *file, gint line);
void ctest_case_assert_equali       (CTestCase *tc, gint i1, gint i2, const gchar *file, gint line);
void ctest_case_assert_equall       (CTestCase *tc, glong l1, glong l2, const gchar *file, gint line);
void ctest_case_assert_equals       (CTestCase *tc, gshort s1, gshort s2, const gchar *file, gint line);
void ctest_case_assert_equalc       (CTestCase *tc, gchar c1, gchar c2, const gchar *file, gint line);
void ctest_case_assert_equalf       (CTestCase *tc, gfloat f1,  gfloat f2,  gfloat tolerance,  const gchar *file, gint line);
void ctest_case_assert_equald       (CTestCase *tc, gdouble g1, gdouble d2, gdouble tolerance, const gchar *file, gint line);
void ctest_case_assert_equal_string (CTestCase *tc, const gchar *s1, const gchar *s2, const gchar *file, gint line);
void ctest_case_assert_not_reached  (CTestCase *tc, const gchar *file, gint line);

/* Test case API macros */

#define ctest_assert(tc, condition, args...)\
	ctest_case_assert(tc, condition, __FILE__, __LINE__, args)

#define ctest_assert_true(tc, condition)\
	ctest_case_assert(tc, condition, __FILE__, __LINE__, "Assertion `" #condition "' failed")

#define ctest_assert_equal(tc, expected, got)\
	ctest_case_assert(tc, (expected)==(got), __FILE__, __LINE__, "Expected `"  #expected "' got `" #got "'")

#define ctest_assert_equali(tc, expected, got)\
	ctest_case_assert_equali (tc, expected, got, __FILE__, __LINE__)

#define ctest_assert_equall(tc, expected, got)\
	ctest_case_assert_equall (tc, expected, got, __FILE__, __LINE__)

#define ctest_assert_equals(tc, expected, got)\
	ctest_case_assert_equals (tc, expected, got, __FILE__, __LINE__)

#define ctest_assert_equalc(tc, expected, got)\
	ctest_case_assert_equalc (tc, expected, got, __FILE__, __LINE__)

#define ctest_assert_equalf(tc, expected, got, tolerance)\
	ctest_case_assert_equalf (tc, expected, got, tolerance, __FILE__, __LINE__)

#define ctest_assert_equald(tc, expected, got, tolerance)\
	ctest_case_assert_equald (tc, expected, got, tolerance, __FILE__, __LINE__)

#define ctest_assert_equal_string(tc, expected, got)\
	ctest_case_assert_equal_string(tc, expected, got, __FILE__, __LINE__)

#define ctest_assert_not_reached(tc)\
	ctest_case_assert_not_reached(tc, __FILE__, __LINE__)


#endif /* __CTEST_H__ */
