#include "server.h"
#include "database.h"

/*
 * Function: delete_all_msgs
 * -------------------------------
 * delete all messages from this room
 * 
 * room_id: room id
 */
void delete_all_msgs(sqlite3 *db, gint room_id) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    gchar *request = NULL;

    sqlite3_str_appendf(sqlite_str, "delete from messages where "
                                    "room_id = %llu", room_id);
    request = sqlite3_str_finish(sqlite_str);
    sqlite3_exec(db, request, 0, 0, 0);
    sqlite3_free(request);
}

/*
 * Function: delete_message_by_id
 * -------------------------------
 * delete message
 * 
 * db: closed database structure
 * id: message id
 */
void delete_message_by_id(sqlite3 *db, guint64 id) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "delete from messages where message_id = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 * Function: delete_room_by_id
 * -------------------------------
 * delete room and members with the room 
 * and messages in the room
 * 
 * db: closed database structure
 * id: room_id
 */
void delete_room_by_id(sqlite3 *db, guint64 room_id) {
    gchar *request = NULL;
    sqlite3_str *sqlite_str = sqlite3_str_new(db);

    sqlite3_str_appendf(sqlite_str, "delete from rooms where id = %llu;"
                                    "delete from members where room_id = %llu;"
                                    "delete from messages where room_id "
                                    "= %llu",
                        room_id, room_id, room_id);
    request = sqlite3_str_finish(sqlite_str);
    sqlite3_exec(db, request, 0, 0, 0);
    sqlite3_free(request);
}

/*
 * Function: delete_user_by_id
 * -------------------------------
 * delete user and contacts with user
 * 
 * db: closed database structure
 * id: user_id
 */
void delete_user_by_id(sqlite3 *db, guint64 id) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    gchar *request = NULL;

    sqlite3_str_appendf(sqlite_str, "delete from users where id = %lu; delete "
                                    "from contacts where user_id = %lu and whe"
                                    "re user_contact_id = %lu;", id, id, id);
    request = sqlite3_str_finish(sqlite_str);
    sqlite3_exec(db, request, 0, 0, 0);
    sqlite3_free(request);
}
