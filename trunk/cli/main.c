#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <glib.h>

#include "pwmonkey.h"

static gchar *domain     = NULL;
static gchar *pass       = NULL;
static int    len        = 16;
static gchar *out_file   = NULL;

static gchar *
read_password (void)
{
	return getpass ("Password: ");
}


static GOptionEntry cmd_options[] = {
	{"domain",   'd', 0, G_OPTION_ARG_STRING,   &domain,   "Domain", "DOM"},
	{"length",   'l', 0, G_OPTION_ARG_INT,      &len,      "Generated password length", "N"},
	{"out",      'o', 0, G_OPTION_ARG_FILENAME, &out_file, "Output to FILE", "FILE"},
	{"password", 'p', 0, G_OPTION_ARG_STRING,   &pass,     "Master password", "PASS"},
	{NULL}
};


int main (int argc, char **argv)
{
	/* Parse command line options */
	GError *error = NULL;
	GOptionContext *context;
	context = g_option_context_new ("- generate a password for domain");
	g_option_context_add_main_entries (context, cmd_options, "pwmonkey");
	//g_option_context_add_group (context, gtk_get_option_group (TRUE));
	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		g_print ("option parsing failed: %s\n", error->message);
      		return EXIT_FAILURE;
	}


	if (domain == NULL)
		domain = argv[1];

	if (domain == NULL) {
		g_printerr ("Usage: %s [options] domain\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (pass == NULL)
		pass = read_password ();

	/*
	g_print ("domain = %s\n", domain);
	g_print ("password = %s\n", pass);
	g_print ("len = %d\n", len);
	g_print ("out = %s\n", out_file);
	*/

	gchar *pw = password_monkey_generate_pass (domain, pass, len);
	memset (pass, 0, strlen (pass));

	g_print ("%s\n", pw);
	return EXIT_SUCCESS;
}

