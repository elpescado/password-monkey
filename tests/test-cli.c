#include <string.h>
#include <stdlib.h>

#include "pwmonkey.h"
#include "ctest.h"
#include "csv.h"

static void
hex2bin (const gchar *hex, guint8 *bin)
{
	size_t len = strlen (hex);
	gint i;

	for (i = 0; i < len/2; i++) {
		gchar tmp[2] = {0};
		strncpy (tmp, hex+2*i, 2);
		bin[i] = (int) strtol (tmp, NULL, 16);
	}
}


void
test_ident (CTestCase *tc, gpointer data)
{
	CsvFile *csv = csv_file_load ("data/ident.csv");

	int i;
	for (i = 0; i < csv_file_num_rows (csv); i++) {
		const gchar *domain = csv_file_get_data (csv, i, 0);
		const gchar *password = csv_file_get_data (csv, i, 1);
		const gchar *expected_id = csv_file_get_data (csv, i, 2);
		gchar *id = password_monkey_ident (domain, password);
		ctest_assert_equal_string (tc, expected_id, id);
		g_free (id);
	}

	csv_file_free (csv);
}


void
test_hash (CTestCase *tc, gpointer data)
{
	CsvFile *csv = csv_file_load ("data/hash.csv");

	int i;
	for (i = 0; i < csv_file_num_rows (csv); i++) {
		const gchar *string = csv_file_get_data (csv, i, 0);
		const gchar *hex_hash = csv_file_get_data (csv, i, 1);
		guint8 expected_hash[32];
		hex2bin (hex_hash, expected_hash);

		guint8 *hash = password_monkey_hash (string);
		
		ctest_assert (tc, memcmp (expected_hash, hash, 32) == 0, "Failurerow %d", i);
		g_free (hash);
	}

	csv_file_free (csv);
}


void
test_encode (CTestCase *tc, gpointer data)
{
	CsvFile *csv = csv_file_load ("data/encode.csv");

	int i;
	for (i = 0; i < csv_file_num_rows (csv); i++) {
		const gchar *hex_hash = csv_file_get_data (csv, i, 0);
		const gchar *expected_b64 = csv_file_get_data (csv, i, 1);
		guint8 hash[32];
		hex2bin (hex_hash, hash);

		gchar *b64 = password_monkey_encode (hash);
		ctest_assert_equal_string (tc, expected_b64, b64);
		g_free (b64);		
	}

	csv_file_free (csv);
}


void
test_truncate (CTestCase *tc, gpointer data)
{
	CsvFile *csv = csv_file_load ("data/truncate.csv");

	int i;
	for (i = 0; i < csv_file_num_rows (csv); i++) {
		const gchar *subject = csv_file_get_data (csv, i, 0);
		const gchar *len_str = csv_file_get_data (csv, i, 1);
		const gchar *expected = csv_file_get_data (csv, i, 2);
		gint len = atoi (len_str);

		gchar *res = password_monkey_truncate (subject, len);
		ctest_assert_equal_string (tc, expected, res);
		g_free (res);		
	}

	csv_file_free (csv);
}



void
test_pwmonkey (CTestCase *tc, gpointer data)
{
	CsvFile *csv = csv_file_load ("data/pwmonkey.csv");

	int i;
	for (i = 0; i < csv_file_num_rows (csv); i++) {
		const gchar *password = csv_file_get_data (csv, i, 0);
		const gchar *domain = csv_file_get_data (csv, i, 1);
		const gchar *len_str = csv_file_get_data (csv, i, 2);
		const gchar *expected = csv_file_get_data (csv, i, 3);
		gint len = atoi (len_str);

		gchar *res = password_monkey_generate_pass (domain, password, len);
		ctest_assert_equal_string (tc, expected, res);
		g_free (res);		
	}

	csv_file_free (csv);
}




int main (int argc, char *argv[])
{
	CTest *test = ctest_new (NULL);

	ctest_add_case (test, test_ident);
	ctest_add_case (test, test_hash);
	ctest_add_case (test, test_encode);
	ctest_add_case (test, test_truncate);
	ctest_add_case (test, test_pwmonkey);

	ctest_run (test);

	return ctest_finish (test);
}
