#include <glib.h>

#include "glib-ext.h"
#include "sys-pedantic.h"
#include <string.h>
#include "cetus-util.h"

/**
 * free function for GStrings in a GHashTable
 */
void g_hash_table_string_free(gpointer data) {
    g_string_free(data, TRUE);
}

/**
 * common GString free func as a hook
 */
void g_string_true_free(gpointer data) {
    g_string_free(data, TRUE);
}


/**
 * hash function for GString
 */
guint g_hash_table_string_hash(gconstpointer _key) {
    return g_string_hash(_key);
}

/**
 * compare function for GString
 */
gboolean g_hash_table_string_equal(gconstpointer _a, gconstpointer _b) {
    return g_string_equal(_a, _b);
}

/**
 * true-function for g_hash_table_foreach
 */
    gboolean 
g_hash_table_true(gpointer UNUSED_PARAM(key), gpointer UNUSED_PARAM(value),
        gpointer UNUSED_PARAM(u)) 
{
    return TRUE;
}	


/**
 * duplicate a GString
 */
GString *g_string_dup(GString *src) {
    GString *dst = g_string_sized_new(src->len+1);

    g_string_assign(dst, src->str);

    return dst;
}

/**
 * compare two strings (gchar arrays), whose lengths are known
 */
gboolean strleq(const gchar *a, gsize a_len, const gchar *b, gsize b_len) {
    if (a_len != b_len) return FALSE;
    return (0 == memcmp(a, b, a_len));
}

/**
 * calculate the difference between two GTimeVal values, in usec
 * positive return value in *tdiff means *told is indeed "earlier" than *tnew,
 * negative return value means the reverse
 * Caller is responsible for passing valid pointers
 */
void ge_gtimeval_diff(GTimeVal *told, GTimeVal *tnew, gint64 *tdiff) {
    *tdiff = (gint64) tnew->tv_sec - told->tv_sec;
    *tdiff *= G_USEC_PER_SEC;
    *tdiff += (gint64) tnew->tv_usec - told->tv_usec;
}

GString *g_string_assign_len(GString *s, const char *str, gsize str_len) {
    g_string_truncate(s, 0);
    return g_string_append_len(s, str, str_len);
}

void g_debug_hexdump(const char *msg, const void *_s, size_t len) {
    GString *hex;
    size_t i;
    const unsigned char *s = _s;

    hex = g_string_new(NULL);

    for (i = 0; i < len; i++) {
        if (i % 16 == 0) {
            g_string_append_printf(hex, "[%04"G_GSIZE_MODIFIER"x]  ", i);
        }
        g_string_append_printf(hex, "%02x", s[i]);

        if ((i + 1) % 16 == 0) {
            size_t j;
            g_string_append_len(hex, C("  "));
            for (j = i - 15; j <= i; j++) {
                g_string_append_c(hex, g_ascii_isprint(s[j]) ? s[j] : '.');
            }
            g_string_append_len(hex, C("\n  "));
        } else {
            g_string_append_c(hex, ' ');
        }
    }

    if (i % 16 != 0) {
        /* fill up the line */
        size_t j;

        for (j = 0; j < 16 - (i % 16); j++) {
            g_string_append_len(hex, C("   "));
        }

        g_string_append_len(hex, C(" "));
        for (j = i - (len % 16); j < i; j++) {
            g_string_append_c(hex, g_ascii_isprint(s[j]) ? s[j] : '.');
        }
    }

    g_debug("(%s) %"G_GSIZE_FORMAT" bytes:\n  %s", 
            msg, 
            len,
            hex->str);

    g_string_free(hex, TRUE);
}

