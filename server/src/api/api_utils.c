#include "server.h"

/*
 * Function: get_member_type
 * -------------------------------
 * determines what rights this user has in this room
 * 
 * user_id: user id
 * room_id: room id
 * 
 * return permissiom user in this room
 */
gint8 get_member_type(sqlite3 *db, gint user_id, gint room_id) {
    sqlite3_stmt *stmt;
    gint32 rv = 0;
    gint8 perm_user = -1;

    rv = sqlite3_prepare_v2(db, "select permission from members where "
                                "room_id = ?1 and user_id = ?2 ", 
                            -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, room_id);
    sqlite3_bind_int64(stmt, 2, user_id);
    if ((rv = sqlite3_step(stmt)) == SQLITE_ROW) {
        perm_user = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return perm_user;
}

/*
 * Function: is_msg_owner
 * -------------------------------
 * checks if the user is the owner of the message
 * 
 * user_id: user id
 * msg_id: message id
 * 
 * return TRUE if user is owner
 */
gboolean is_msg_owner(sqlite3 *db, gint user_id, gint msg_id) {
    sqlite3_stmt *stmt;
    gint32 rv = 0;
    gboolean is_owner = false;

    rv = sqlite3_prepare_v2(db, "select * from messages where message_id = ?1 "
                                "and user_id = ?2 ", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, msg_id);
    sqlite3_bind_int64(stmt, 2, user_id);
    if ((rv = sqlite3_step(stmt)) == SQLITE_ROW) {
        is_owner = true;
    }
    sqlite3_finalize(stmt);
    return is_owner;
}

/*
 * Function: is_member
 * -------------------------------
 * checks for a record of the connection of this 
 * user with this room
 * 
 * user_id: user id
 * room_id: room id
 * 
 * return: TRUE or FALSE
 */
gboolean is_member(sqlite3 *db, gint user_id, gint room_id) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "select * from members where user_id = ?1 "
                                "and room_id = ?2", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_int64(stmt, 2, room_id);
    if ((rv = sqlite3_step(stmt)) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

gboolean is_valid_info(gchar *msg, gsize max_value) {
    gsize len = 0;

    if (!msg)
        return false;
    len = strlen(msg);
    if (len <= 0 || len > max_value)
        return false;
    return true;
}

static void randomize_str(gchar *str, size_t count) {
    for (size_t i = 0; i < count; i++) {
        str[i] = (gchar)g_random_int_range(1, CHAR_MAX);
    }
}

/*
 * Function: create_user_token
 * -------------------------------
 * Creates SHA-256 token
 * 
 * token: pointer to string that need to fill
 * login: client`s login
 */
gchar *create_user_token(char *login) {
    gchar *seed = g_compute_checksum_for_string(G_CHECKSUM_MD5, login, strlen(login));
    gchar *token = NULL;
    gchar *hash = NULL;

    randomize_str(seed, strlen(login) / 2);
    hash = g_compute_checksum_for_string(G_CHECKSUM_SHA256, seed, strlen(seed));
    token = g_strjoin("", hash, login, NULL);
    
    g_free(hash);
    g_free(seed);
    /////maybe strdup(token);
    return strdup(token);
}

gboolean is_valid_room_name(gchar *roomname) {
    size_t len = strlen(roomname);
    if (!roomname) {
        return false;
    }
    if (len <= 0 || len > MAX_ROOM_LEN) {
        return false;
    }
    return true;
}
