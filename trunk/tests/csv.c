#include <stdio.h>

#include "csv.h"

CsvFile *
csv_file_new (void)
{
	CsvFile *csv = g_new0 (CsvFile, 1);

	csv->separator_char = ',';
	csv->rows = g_new0 (CsvRecord*, 1024);

	return csv;
}


CsvFile *
csv_file_load (const gchar *file_name)
{
	FILE *f = fopen (file_name, "r");
	if (f == NULL)
		return NULL;

	CsvFile *csv = csv_file_new ();

	char buffer[1024];

	while (fgets (buffer, 1024, f)) {
		g_strchomp (buffer);
		csv_file_add_raw_row (csv, buffer);
	}

	return csv;
}


void
csv_file_free (CsvFile *csv)
{
}


void
csv_file_add_raw_row (CsvFile     *csv,
                      const gchar *raw)
{
	CsvRecord *row = g_new0 (CsvRecord, 1);

	row->data = g_strdup (raw);
	row->cells = g_new0 (char*, 16);
	row->cells[0] = row->data;
	int cell = 1;
	char *c = row->data;
	while (*c) {
		if (*c == csv->separator_char) {
			*c = '\0';
			row->cells[cell++] = c+1;
		}
		c++;
	}

	csv->rows[csv->row_count++] = row;
}


gint
csv_file_num_rows (CsvFile *csv)
{
	return csv->row_count;
}


const gchar *
csv_file_get_data (CsvFile *csv,
                   gint     row,
                   gint     column)
{
	return csv->rows[row]->cells[column];
}

