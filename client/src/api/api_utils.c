#include "client.h"

/*
 * Function: encrypt_str
 * -------------------------------
 * returns the encrypted str
 */
gchar *encrypt_str(gchar *str) {
    return g_compute_checksum_for_string(G_CHECKSUM_SHA256, str, strlen(str));   
}

/*
 * Function: match_nsearch
 * -------------------------------
 * Search for a regular expression <regex> in a string <str> of <size> characters
 */
bool match_nsearch(char *str, char *regex, size_t size) {
    char buf[size + 1];
    GRegex *g_reg = NULL;
    GMatchInfo *match_info;
    bool g_res;

    buf[size] = 0;
    g_strlcpy(buf, str, size + 1);
    g_reg = g_regex_new(regex, 0, 0, NULL);
    g_res = g_regex_match(g_reg, buf, 0, &match_info);
    g_match_info_free(match_info);
    return g_res;
}
/*
 * Function: match_search
 * -------------------------------
 * Search for a regular expression <regex> in a string <str>
 */
bool match_search(char *str, char *regex) {
    GRegex *g_reg = NULL;
    GMatchInfo *match_info;
    bool g_res;

    g_reg = g_regex_new(regex, 0, 0, NULL);
    g_res = g_regex_match(g_reg, str, 0, &match_info);
    g_match_info_free(match_info);
    g_regex_unref(g_reg);
    return g_res;
}

bool is_valid_auth_data(char *login, char *pass, GtkLabel *label) {
    if (*login == '\0' || *pass == '\0') {
        gtk_label_set_text(label, ERRMSG_NODATA);
        return false;
    }
    else if (match_search(login, VM_LOGIN_REGEX) == 0) {
        gtk_label_set_text(label, ERRMSG_INVALID_LOGIN);
        return false;
    }
    return true;
}
