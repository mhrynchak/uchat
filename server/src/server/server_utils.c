#include "server.h"

/*
 * Function: get_members_list
 * -------------------------------
 * finds and writes in GList logins of users which are in the given room
 * and are not forbidden
 * 
 * room_id: room id
 * 
 * return: GList with login users
 */
GList *get_members_list(sqlite3 *db, gint room_id) {
    sqlite3_stmt *stmt;
    GList *list = NULL;

    sqlite3_prepare_v2(db, "select distinct login from users inner join "
                           "members on users.id = members.user_id where "
                           "room_id = ?1 and permission != ?2", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, DB_BANNED);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        list = g_list_append(list, strdup((char*)sqlite3_column_text(stmt,0)));
    }
    sqlite3_finalize(stmt);
    return list;
}

static void send_to_all(gpointer list, gpointer user_data) {
    t_send_helper *send_helper = (t_send_helper*)user_data;
    GHashTable *table = send_helper->table;
    GDataOutputStream *out = g_hash_table_lookup(table, list);

    if (out) {
        vm_send(out, send_helper->j_data);
    }
    (void)list;
}

/*
 * Function: vm_send_to_all
 * -------------------------------
 * Sends request to all members in room
 * 
 * data: request that need to send
 * client: client that sending request
 * room_id: members of this room will receive this request
 * 
 * returns: new request
 */
void vm_send_to_all(gchar *j_data, t_client *client, gint room_id) {
    GList *list = get_members_list(client->info->database, room_id);
    t_send_helper *send_helper = g_malloc0(sizeof(t_send_helper));

    send_helper->j_data = j_data;
    send_helper->table = client->info->users;
    g_list_foreach(list, send_to_all, send_helper);
    g_free(send_helper);
}
