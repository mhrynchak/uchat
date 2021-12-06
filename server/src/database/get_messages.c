#include "server.h"

/*
 * Function:  get_object_message
 * -------------------------------
 * creates an json object with message data
 * 
 * stmt: sqlite request
 * 
 * return: json object
 */
cJSON *get_object_message(sqlite3_stmt *stmt) {
    cJSON *msg = cJSON_CreateObject();

    cJSON_AddNumberToObject(msg, "msg_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddNumberToObject(msg, "user_id", sqlite3_column_int64(stmt, 1));
    cJSON_AddNumberToObject(msg, "room_id", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(msg, "date", sqlite3_column_int64(stmt, 3));
    cJSON_AddStringToObject(msg, "msg", VM_J_STR((char*)sqlite3_column_text(stmt, 4)));
    cJSON_AddNumberToObject(msg, "msg_type", sqlite3_column_int(stmt, 5));
    cJSON_AddNumberToObject(msg, "file_size", sqlite3_column_int(stmt, 6));
    cJSON_AddStringToObject(msg, "file_name", VM_J_STR((char*)sqlite3_column_text(stmt, 7)));
    cJSON_AddNumberToObject(msg, "status", sqlite3_column_int(stmt, 8));
    return msg;
}

/*
 * Function: get_messages_by_id
 * -------------------------------
 * 
 */
static cJSON *get_messages_by_id(sqlite3_stmt *stmt, gint room_id, 
                                 gint64 count, gint date) {
    cJSON *room = cJSON_CreateObject();
    cJSON *message = cJSON_CreateArray();

    sqlite3_bind_int64(stmt, 1, date);
    for (gint64 i = 0; i < count && sqlite3_step(stmt) == SQLITE_ROW; i++) {
        cJSON_AddItemToArray(message, get_object_message(stmt));
    }
    cJSON_AddItemToObject(room, "room_id", cJSON_CreateNumber(room_id));
    cJSON_AddItemToObject(room, "messages", message);
    sqlite3_finalize(stmt);
    return room;
}

/*
 * Function: get_new_message_by_id
 * -------------------------------
 * create json object with old message data,
 * messages will not be considered at this time
 * 
 * room_id: room id
 * date: time in milisecond
 * count: the number of messages to receive
 * 
 * return json object *
 */
cJSON *get_new_messages_by_id(sqlite3 *db, gint room_id,
                                 gint date, gint64 count) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;
    gchar *request = create_request_message_by_id(db, room_id, DB_NEW_MESSAGE);

    rv = sqlite3_prepare_v2(db, request, -1, &stmt, NULL);
    sqlite3_free(request);
    return get_messages_by_id(stmt, room_id, count, date);
}

/*
 * Function: get_old_messages_by_id
 * -------------------------------
 * create json object with old message data,
 * messages after this time will not be considered
 * 
 * room_id: room id
 * date: time in milisecond
 * count: the number of messages to receive
 * 
 * return json object
 */
cJSON *get_old_messages_by_id(sqlite3 *db, gint room_id,
                              gint date, gint64 count) {
    sqlite3_stmt *stmt;
    gchar *request = create_request_message_by_id(db, room_id, DB_OLD_MESSAGE);
    sqlite3_prepare_v2(db, request, -1, &stmt, NULL);
    sqlite3_free(request);
    return get_messages_by_id(stmt, room_id, count, date);
}

