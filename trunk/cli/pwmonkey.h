#include <glib.h>


gchar *
password_monkey_ident (const gchar *domain,
                       const gchar *password);

guint8 *
password_monkey_hash (const gchar *data);


gchar *
password_monkey_encode (const guint8 *digest);


gchar *password_monkey_truncate (const gchar *encoded,
                                 guint        len);

gchar *
password_monkey_generate_pass (const gchar *domain,
                               const gchar *password,
                               int          len);

