#ifndef __CSV_H__
#define __CSV_H__

#include <glib.h>


typedef struct {
	gchar  *data;
	gchar **cells;
} CsvRecord;


typedef struct {
	gchar       separator_char;
	gint        row_count;
	CsvRecord **rows;	
} CsvFile;


CsvFile *
csv_file_new (void);


CsvFile *
csv_file_load (const gchar *file_name);


void
csv_file_free (CsvFile *csv);


void
csv_file_add_raw_row (CsvFile     *csv,
                      const gchar *row);


gint
csv_file_num_rows (CsvFile *csv);


const gchar *
csv_file_get_data (CsvFile *csv,
                   gint     row,
                   gint     column);


#endif /* __CSV_H__ */
