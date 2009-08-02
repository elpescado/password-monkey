#include <string.h>
#include <stdlib.h>

#include <glib.h>

#ifdef USE_OPENSSL
#  include <openssl/sha.h>
#endif

#include "pwmonkey.h"


#define PWMON_ID "5dfabc489612601f0e253781fbc6f009"

gchar *
password_monkey_generate_pass (const gchar *domain,
                               const gchar *password,
                               int          len)
{
	/* Generate identifier */
	gchar *data = g_strdup_printf ("pwmon(%s;%s;%s)",
                                       domain,
                                       password,
                                       PWMON_ID);

	/* Compute checksum */
#ifdef USE_OPENSSL
	guint8 *digest = g_new (guint8, SHA_DIGEST_LENGTH);
	SHA1 (data, strlen (data), digest);
	gsize digest_len = SHA_DIGEST_LENGTH;
#elif
	GChecksumType hash_type = G_CHECKSUM_SHA1;
	guint8 *digest = g_new (guint8, hash_len);
	GChecksum *hash = g_checksum_new (hash_type);
	g_checksum_update (hash, (guchar*) data, strlen (data));
	gssize hash_len = g_checksum_type_get_length (hash_type);
	gsize digest_len = hash_len;
	g_checksum_get_digest (hash, digest, &digest_len);
	g_checksum_free (hash);
#endif


	g_free (data);

	/* Encode as Base-64 and trim */
	gchar *encoded = g_base64_encode (digest, digest_len);
	gchar *trimmed = g_strndup (encoded, len);
	g_free (encoded);

	return trimmed;
}
