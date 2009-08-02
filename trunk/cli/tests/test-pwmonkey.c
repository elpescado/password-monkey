#include "ctest.h"

#include "pwmonkey.h"

/* Data item */
struct di {
	char    *master_password;
	char    *domain;
	int      length;

	char    *pass;
};

struct di test_data[] = {
#include "data.dat"
};

void
test_pwmonkey (CTestCase *tc, gpointer test_data)
{
	struct di *data = ctest_case_get_data (tc);
	gchar *pwd = password_monkey_generate_pass (data->domain,
	                                            data->master_password,
						    data->length);

	ctest_assert_equal_string (tc, data->pass, pwd);
	ctest_assert_equali (tc, data->length, strlen (pwd));
	g_free (pwd);
}

int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);
	ctest_add_case_dataset (test, test_pwmonkey, test_data);
	ctest_run (test);
	return ctest_finish (test);
}
