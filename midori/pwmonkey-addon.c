#include <midori/midori.h>
#include <webkit/webkit.h>
#include <gtk/gtk.h>


static void
pwmonkey_addon_fill_password_entries (WebKitWebView *web_view,
                                      const gchar   *password)
{
	const gchar *script_tpl = ""
		"var inputs = document.getElementsByTagName ('input');"
		"for (var i in inputs)"
		"	if (inputs[i].type == 'password')"
		"		inputs[i].value = '%s';";

	gchar *script = g_strdup_printf (script_tpl, password);
	webkit_web_view_execute_script (web_view, script);
	g_free (script);
}


static gchar *
pwmonkey_addon_get_domain_name (WebKitWebView *web_view)
{
	WebKitWebFrame *frame = webkit_web_view_get_main_frame (web_view);
	const gchar *uri = webkit_web_frame_get_uri (frame);

	g_print ("Trying to get domain for '%s'\n", uri);
	GError *error = NULL;
	GRegex *regex = g_regex_new ("^[a-z]+://([a-z.]+)/", 0, 0, &error);
	if (regex == NULL) {
		return NULL;
	}

	gchar *domain = NULL;
	GMatchInfo *match_info = NULL;
	g_regex_match (regex, uri, 0, &match_info);
	if (g_match_info_matches (match_info)) {
		domain = g_match_info_fetch (match_info, 1);
		g_print (" -> '%s'\n", domain);
	}
	g_match_info_free (match_info);
	g_regex_unref (regex);

	return domain;
	
}


static void
pwmonkey_addon_menuitem_activate_cb (GtkMenuItem   *menuitem,
                                     WebKitWebView *web_view)
//                                     MidoriExtension *extension)
{
	GtkWidget *dialog;
	GtkWidget *table;
	GtkWidget *domain_entry;
	GtkWidget *master_password_entry;
	GtkWidget *length_entry;

	/* Create Password Monkey dialog */
	dialog = gtk_dialog_new_with_buttons (_("Password Monkey"),
                                              GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (web_view))),
	                                      GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
	                                      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                      NULL);
	table = gtk_table_new (3, 2, FALSE);
	gtk_table_set_row_spacings (GTK_TABLE (table), 6);
	gtk_table_set_col_spacings (GTK_TABLE (table), 6);
	gtk_container_set_border_width (GTK_CONTAINER (table), 12);
	
	gtk_table_attach (GTK_TABLE (table), gtk_label_new ("Master password:"),
	                  0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
	master_password_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (table), master_password_entry,
	                  1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_SHRINK, 0, 0);

	gtk_table_attach (GTK_TABLE (table), gtk_label_new ("Domain:"),
	                  0, 1, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
	domain_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (table), domain_entry,
	                  1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_SHRINK, 0, 0);

	gtk_table_attach (GTK_TABLE (table), gtk_label_new ("Password length:"),
	                  0, 1, 2, 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
	length_entry = gtk_entry_new ();
	gtk_table_attach (GTK_TABLE (table), length_entry,
	                  1, 2, 2, 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
	
	gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), table);	

	gtk_widget_show_all (table);

	gchar *domain = pwmonkey_addon_get_domain_name (web_view);
	gtk_entry_set_text (GTK_ENTRY (domain_entry), domain);
	g_free (domain);

	int res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		pwmonkey_addon_fill_password_entries (web_view, "alamakota");
	}
	gtk_widget_destroy (dialog);
}


static void
pwmonkey_addon_populate_popup_cb (WebKitWebView   *web_view,
                                  GtkMenu         *menu,
                                  MidoriExtension *extension)
{
	GtkWidget *item;

	item = gtk_menu_item_new_with_label (_("Password Monkey"));
	g_signal_connect (G_OBJECT (item), "activate",
	                  G_CALLBACK (pwmonkey_addon_menuitem_activate_cb), web_view);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
	gtk_widget_show (item);
}



static void
pwmonkey_addon_add_tab_cb (MidoriBrowser   *browser,
                           MidoriView      *view,
                           MidoriExtension *extension)
{
	g_print ("ADD-TAB\n");

	GtkWidget *child;
	child = gtk_bin_get_child (GTK_BIN (view));

	if (WEBKIT_IS_WEB_VIEW (child)) {
		WebKitWebView *web_view = WEBKIT_WEB_VIEW (child);

		g_signal_connect (G_OBJECT (web_view), "populate_popup",
                                  G_CALLBACK (pwmonkey_addon_populate_popup_cb), extension);
	}
}


static void
pwmonkey_addon_add_browser_cb (MidoriApp       *app,
                               MidoriBrowser   *browser,
                               MidoriExtension *extension)
{
	g_signal_connect (browser, "add-tab", G_CALLBACK (pwmonkey_addon_add_tab_cb), extension);
}


static void
pwmonkey_addon_activate_cb (MidoriExtension *extension,
                            MidoriApp       *app,
                            gpointer         data)
{
	KatzeArray* browsers;
	MidoriBrowser* browser;
	guint i = 0;

	browsers = katze_object_get_object (app, "browsers");
	while ((browser = katze_array_get_nth_item (browsers, i++))) {
		pwmonkey_addon_add_browser_cb (app, browser, extension);
	}

	g_signal_connect (app, "add-browser", G_CALLBACK (pwmonkey_addon_add_browser_cb), extension);

}


static void
pwmonkey_addon_deactivate_cb (MidoriExtension *extension,
                              gpointer         data)
{
	g_print ("quit\n");
}


MidoriExtension*
extension_init (void)
{
	MidoriExtension* extension = g_object_new (MIDORI_TYPE_EXTENSION,
		"name", _("Password Monkey"),
		"description", _("Simple password utility"),
		"version", "0.1",
		"authors", "Przemysław Sitek",
		NULL);

	g_signal_connect (extension, "activate", G_CALLBACK (pwmonkey_addon_activate_cb), NULL);
	g_signal_connect (extension, "deactivate", G_CALLBACK (pwmonkey_addon_deactivate_cb), NULL);

	return extension;
}

