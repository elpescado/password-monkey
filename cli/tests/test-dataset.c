#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ctest.h"

struct testitem {
	int num;
	const char *str;
};

struct testitem sprintf_data[] = {
	{0,           "0"},
	{1,           "1"},
	{2,           "2"},
	{-1,          "-1"},
	{-2,          "-2"},
	{2147483647,  "2147483647"},
	{-2147483648, "-2147483648"},
	{7, "7"}
};


void
test_sprintf (CTestCase *tc, gpointer test_data)
{
	const struct testitem *data = ctest_case_get_data (tc);

	char buffer[32];

	sprintf (buffer, "%d", data->num);

	ctest_assert_equal_string (tc, data->str, buffer);
}


void
test_atoi (CTestCase *tc, gpointer test_data)
{
	const struct testitem *data = ctest_case_get_data (tc);

	int i = atoi (data->str);

	ctest_assert_equali (tc, data->num, i);
}


void
test_strtol (CTestCase *tc, gpointer test_data)
{
	const struct testitem *data = ctest_case_get_data (tc);

	char *end;
	long int i = strtol (data->str, &end, 10);

	ctest_assert_equall (tc, data->num, i);
	ctest_assert_equal_string (tc, "", end);
}



int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case_dataset (test, test_sprintf, sprintf_data);
	ctest_add_case_dataset (test, test_atoi, sprintf_data);
	ctest_add_case_dataset (test, test_strtol, sprintf_data);

	ctest_run (test);

	return ctest_finish (test);
}

