#include "server.h"

/*
 * Function: edit_room_desc_by_id
 * -------------------------------
 * changes the name of the room
 * 
 * id: room_id
 * new_name: new room name
 */
void edit_room_desc_by_id(sqlite3 *db, gint id, gchar *new_name) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "update rooms set desc = ?1 "
                            "where id = ?2", -1,
                            &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 * Function: edit_room_name_by_id
 * -------------------------------
 * changes the name of the room
 * 
 * id: room_id
 * new_name: new room name
 */
void edit_room_name_by_id(sqlite3 *db, gint id, gchar *new_name) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update rooms set name = ?1 "
                                "where id = ?2", -1,
                            &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 * Function: edit_user_name_by_id
 * -------------------------------
 * changes the name of the user
 * 
 * id: user_id
 * new_name: new user name
 */
void edit_user_name_by_id(sqlite3 *db, gint id, gchar *new_name) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update users set login = ?1 "
                                "where id = ?2", -1,
                            &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 * Function: edit_message_by_id
 * -------------------------------
 * changes the message
 * 
 * id: message id
 * new: new message
 */
void edit_message_by_id(sqlite3 *db, gint id, gchar *new) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "update messages set message = ?1 "
                                "where message_id = ?2", -1,
                            &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 * Function: edit_type_member
 * -------------------------------
 * changes permission of user in this room
 * 
 * room_id: room id
 * user_id: user id
 * new type: new permission
 */
void edit_member_type(sqlite3 *db, gint room_id, gint user_id, gint8 new_type) {
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    gchar *request = NULL;

    sqlite3_str_appendf(sqlite_str, "update members set permission = %d where "
                                    "room_id = %llu and user_id = %llu",
                        new_type, room_id, user_id);
    request = sqlite3_str_finish(sqlite_str);
    sqlite3_exec(db, request, 0, 0, 0);
    sqlite3_free(request);
}
