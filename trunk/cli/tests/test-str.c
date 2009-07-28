#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ctest.h"

void
test_strcpy (CTestCase *tc, gpointer data)
{
	char buffer[12];
	strcpy (buffer, "qwerty");

	ctest_assert_equal_string (tc, "qwerty", buffer);
}


void
test_strcat (CTestCase *tc, gpointer data)
{
	const gchar *s1 = "abcde";
	const gchar *s2 = "12345";

	char buffer[12];
	strcpy (buffer, s1);
	strcat (buffer, s2);

	ctest_assert_equal_string (tc, "abcde12345", buffer);
}



int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case (test, test_strcpy);
	ctest_add_case (test, test_strcat);

	ctest_run (test);

	//ctest_free (test);

	//return EXIT_SUCCESS;
	return ctest_finish (test);
}

