#include <string.h>
#include <stdlib.h>

#include <glib.h>

#ifdef USE_OPENSSL
#  include <openssl/sha.h>
#endif

#include "pwmonkey.h"


#define PWMON_ID "5dfabc489612601f0e253781fbc6f009"

static void
_print_hex_hash (const guint8 *data, size_t len)
{
	int i;
	for (i = 0; i < len; i++) {
		g_print ("%02x", (int)data[i]);
	}
	g_print ("\n");
}


/**
 * IDENT operation
 *
 * Return: newly allocated string to be freed with g_free()
 **/
gchar *
password_monkey_ident (const gchar *domain,
                       const gchar *password)
{
	return g_strdup_printf ("pwmon(%s;%s;%s)",
                                domain,
                                password,
                                PWMON_ID);
}


/**
 * HASH operation
 *
 * Return: Newly allocated buffer
 **/
guint8 *
password_monkey_hash (const gchar *data)
{
#ifdef USE_OPENSSL
/*
	guint8 *digest = g_new (guint8, SHA_DIGEST_LENGTH);
	SHA1 (data, strlen (data), digest);
	gsize digest_len = SHA_DIGEST_LENGTH;
*/
#else
	GChecksumType hash_type = G_CHECKSUM_SHA256;
	gssize hash_len = g_checksum_type_get_length (hash_type);
	guint8 *digest = g_new (guint8, hash_len);
	GChecksum *hash = g_checksum_new (hash_type);
	g_checksum_update (hash, (guchar*) data, strlen (data));
	gsize digest_len = hash_len;
	g_checksum_get_digest (hash, digest, &digest_len);
	g_checksum_free (hash);
#endif

	return digest;
}


gchar *
password_monkey_encode (const guint8 *digest)
{
	return g_base64_encode (digest, 32);
}


gchar *password_monkey_truncate (const gchar *encoded,
                                 guint        len)
{
	return g_strndup (encoded, len);
}


gchar *
password_monkey_generate_pass (const gchar *domain,
                               const gchar *password,
                               int          len)
{
	gchar *id = password_monkey_ident (domain, password);
	guint8 *hash = password_monkey_hash (id); g_free (id);
	gchar *encoded = password_monkey_encode (hash); g_free (hash);
	gchar *trimmed = password_monkey_truncate (encoded, len); g_free (encoded);
	return trimmed;
}

