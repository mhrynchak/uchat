#include "server.h"
#include "database.h"

/*
 * Function: get_text_message
 * -------------------------------
 * returns the message text from the messages tab
 * 
 * message_id: message id
 * 
 * return: text in gchar
 */
gchar *get_message_by_id(sqlite3 *db, guint64 message_id) {
    sqlite3_stmt *stmt;
    gchar *result = NULL;

    sqlite3_prepare_v2(db, "select message from messages where message_id = ?1", -1, &stmt, 0);
    sqlite3_bind_int64(stmt, 1, message_id);
    sqlite3_step(stmt);
    if (sqlite3_column_text(stmt, 0) != NULL) {
        result = strdup((const char*)sqlite3_column_text(stmt, 0));
    }
    return result;
}

/*
 * Function: 
 * 
 */
static void get_id_msg(sqlite3 *db, t_db_message *message) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    rv = sqlite3_prepare_v2(db, "select message_id from messages where "
                                "room_id = ?1 and date = ?2",
                            -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, message->room_id);
    sqlite3_bind_int64(stmt, 2, message->date);
    sqlite3_step(stmt);
    message->message_id = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
}

/*
 * Function: 
 * 
 */
static void sqlite_bind_msg(sqlite3_stmt *stmt, t_db_message *message) {
    sqlite3_bind_int(stmt, 1, message->user_id);
    sqlite3_bind_int(stmt, 2, message->room_id);
    sqlite3_bind_int(stmt, 3, message->date);
    sqlite3_bind_text(stmt, 4, message->message, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, message->type);
    sqlite3_bind_int(stmt, 6, message->file_size);
    sqlite3_bind_text(stmt, 7, message->file_name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, message->status);
}

/*
 * Function: insert_message
 * -------------------------------
 * makes an entry in the database in the messages table
 * in which enters data from the structure t_db_message
 * 
 * room: structure t_db_message with filled fields 
 * except room_id and date and status
 * 
 * return: complements the structure of t_db_message
 */
void insert_message_into_db(sqlite3 *db, t_db_message *message) {
    sqlite3_stmt *stmt;
    gint rv;

    message->date = vm_get_time(DB_MILISECOND);
    message->status = DB_MSG_START;
    rv = sqlite3_prepare_v2(db, "INSERT INTO MESSAGES(USER_ID, ROOM_ID, "
                           "DATE, MESSAGE, TYPE, FILE_SIZE, FILE_NAME, STATUS)"
                           "VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8);",
                            -1, &stmt, NULL);
    sqlite_bind_msg(stmt, message);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    get_id_msg(db, message);
}

/*
 * Function: get_room_by_id
 * -------------------------------
 * get data of room in t_db_room
 * 
 * id: room id
 * 
 * return: struct t_db_room with data of room
 */
t_db_room *get_room_by_id(sqlite3 *db, guint64 id) {
    t_db_room *room = NULL;
    sqlite3_stmt *stmt;

    sqlite3_prepare_v3(db, "SELECT * FROM ROOMS WHERE ID = ?1", -1, 0, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == 100) {
        room = vm_malloc(sizeof(t_db_room));
        room->room_id = sqlite3_column_int64(stmt, 0);
        room->room_name = strdup((const char*)sqlite3_column_text(stmt, 1));
        room->user_id = sqlite3_column_int64(stmt, 2);
        room->date = sqlite3_column_int64(stmt, 3);
        room->desc = strdup(VM_J_STR((char*)sqlite3_column_text(stmt, 4)));
        room->type = sqlite3_column_int(stmt, 5);
    }
    sqlite3_finalize(stmt);
    return room;
}

static gchar *parse_str_search(gchar *str) {
    gchar *tmp = NULL;
    gint size = (gint)strlen(str);
    gint tmp_point = 0;

    for (gint i = 0; i < (gint)strlen(str); i++) {
        if (str[i] == '%' || str[i] == '_' || str[i] == '\\')
            size++;
    }
    tmp = malloc(size + 1);
    tmp[size] = 0;
    for (gint i = 0; i < (gint)strlen(str); i++, tmp_point++) {
        if (str[i] == '%' || str[i] == '_' || str[i] == '\\') {
            tmp[tmp_point++] = '\\';
            tmp[tmp_point] = str[i];
        }
        else
            tmp[tmp_point] = str[i];
    }
    return tmp;
}

cJSON *search_msgs_in_db(sqlite3 *db, gint room_id, gchar *str_search) {
    gchar *str_search_join = NULL;
    sqlite3_stmt *stmt;
    cJSON *messages = cJSON_CreateArray();

    str_search = parse_str_search(str_search);
    str_search_join = g_strjoin("", "%%", str_search, "%%", NULL);
    sqlite3_prepare_v2(db, "SELECT * FROM MESSAGES WHERE ROOM_ID = ?2"
                           " AND MESSAGE LIKE ?1 ESCAPE '\\' AND TYPE = ?3",
                       -1, &stmt, NULL),
    sqlite3_bind_text(stmt, 1, str_search_join, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, room_id);
    sqlite3_bind_int(stmt, 3, DB_TEXT_MSG);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON_AddItemToArray(messages, get_object_message(stmt));
    }
    g_free(str_search_join);
    // free((void**)&str_search);                    ///fix later
    sqlite3_finalize(stmt);
    return messages;
}

/*
 * Function: get_object_room
 * -------------------------------
 * creates a json object with room data
 * 
 * stmt: sqlite request
 * 
 * return: json object
 */
static cJSON *get_object_room(sqlite3_stmt *stmt) {
    cJSON *room = cJSON_CreateObject();

    cJSON_AddNumberToObject(room, "id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(room, "name", VM_J_STR((char*)sqlite3_column_text(stmt, 1)));
    cJSON_AddNumberToObject(room, "user_id", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(room, "date", sqlite3_column_int64(stmt, 3));
    cJSON_AddStringToObject(room, "desc", VM_J_STR((char*)sqlite3_column_text(stmt, 4)));
    cJSON_AddNumberToObject(room, "type", sqlite3_column_int(stmt, 5));

    return room;
}

/*
 * Function: get_rooms
 * -------------------------------
 * returns all user rooms where he can be active
 * rooms that have been created so far are not taken into account
 * 
 * date: time in milisecond
 * user_id: user id
 * 
 * return: json object
 */
cJSON *get_json_rooms(sqlite3 *db, guint64 date, guint64 user_id) {
    cJSON *j_rooms = cJSON_CreateArray();
    sqlite3_stmt *stmt;
    
    sqlite3_prepare_v2(db, "SELECT * FROM ROOMS WHERE ID IN (SELECT ROOM"
                           "_ID FROM MEMBERS WHERE USER_ID = ?2 AND "
                           "PERMISSION != ?3) AND DATE > ?1 ORDER BY "
                           "DATE DESC",
                       -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, date);
    sqlite3_bind_int64(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, DB_BANNED);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON_AddItemToArray(j_rooms, get_object_room(stmt));
    }
    sqlite3_finalize(stmt);
    return j_rooms;
}

/*
 * Function: search_room
 * -------------------------------
 * finds and writes to json an array of room names whose name begins with 
 * str_search and to which this user belongs
 * 
 * str_search: the name of the room they are looking for
 * user_id: user id
 * 
 * return: json object
 */
cJSON *search_rooms_in_db(sqlite3 *db, gint user_id, gchar *str_search) {
    sqlite3_stmt *stmt;
    gchar *str_search_join = NULL;
    cJSON *rooms = cJSON_CreateArray();

    str_search = parse_str_search(str_search);
    str_search_join = g_strjoin("", str_search, "%%", NULL);
    sqlite3_prepare_v2(db, "SELECT * FROM ROOMS WHERE NAME LIKE ?1 ESCAPE"
                           " '\\' AND ID NOT IN(SELECT ROOM_ID FROM MEMBE"
                           "RS WHERE USER_ID = ?2)",
                           -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, str_search_join, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, user_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON_AddItemToArray(rooms, get_object_room(stmt));
    }
    g_free(str_search_join);
    // free((void **)&str_search);           ///change this later
    sqlite3_finalize(stmt);
    return rooms;
}

/*
 * Function:
 * -------------------------------
 * 
 */
static t_db_user *get_user(sqlite3_stmt *stmt) {
    t_db_user *user = NULL;

    if (sqlite3_step(stmt) == 100) {
        user = vm_malloc(sizeof(t_db_user));
        user->user_id = sqlite3_column_int64(stmt, 0);
        user->name = strdup((const char *)sqlite3_column_text(stmt, 1));
        user->login = strdup((const char*)sqlite3_column_text(stmt, 2));
        user->pass = strdup((const char*)sqlite3_column_text(stmt, 3));
        user->auth_token = strdup((const char*)sqlite3_column_text(stmt, 4));
        user->date = sqlite3_column_int(stmt, 5);
        user->desc = strdup(VM_J_STR((char*)sqlite3_column_text(stmt, 6)));
    }
    sqlite3_finalize(stmt);
    return user;
}

/*
 * Function: get_user_by_id
 * -------------------------------
 * get data of user in t_db_user
 * 
 * user_id: user id
 * 
 * return: struct t_db_room with data of user
 */
t_db_user *get_user_by_id(sqlite3 *db, guint64 user_id) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v3(db, "select * from users where id = ?1", -1, 0, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, user_id);
    return get_user(stmt);
}

/*
 * Function: get_user_by_token
 * -------------------------------
 * get data of user in t_db_user
 * 
 * token: token of user
 * 
 * return: struct t_db_room with data of user
 */
t_db_user *get_user_by_token(sqlite3 *db, gchar *token) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v3(db, "SELECT * FROM USERS WHERE AUTH_TOKEN = ?1", -1, 0, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, token, -1, SQLITE_STATIC);
    return get_user(stmt);
}

/*
 * Function: get_user_by_login
 * -------------------------------
 * get data of user in t_db_user
 * 
 * login: login of user
 * 
 * return: struct t_db_room with data of user
 */
t_db_user *get_user_by_login(sqlite3 *db, gchar *login) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v3(db, "SELECT * FROM USERS WHERE LOGIN = ?1", -1, 0, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);

    return get_user(stmt);
}

void get_user_id(sqlite3 *db, t_db_user *user) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE DATE = ?1", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, user->date);
    sqlite3_step(stmt);
    user->user_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
}

/*
 * Function: get_object_members
 * 
 */
static cJSON *get_object_member(sqlite3_stmt *stmt) {
    cJSON *member = cJSON_CreateObject();

    cJSON_AddNumberToObject(member, "id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(member, "login", 
                            (char *)sqlite3_column_text(stmt, 1));
    cJSON_AddNumberToObject(member, "type", sqlite3_column_int(stmt, 2));
    return member;
}

/*
 * Function: get_json_members
 * -------------------------------
 * retrieves from the database and enters in json an 
 * array of all user data that is in the room
 * 
 * room_id: room id
 * 
 * return: json object
 */
cJSON *get_json_members(sqlite3 *db, gint room_id) {
    cJSON *members = cJSON_CreateArray();
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID, LOGIN, PERMISSION FROM USERS INNER "
                           "JOIN MEMBERS ON USERS.ID = MEMBERS.USER_ID "
                           "WHERE ROOM_ID = ?1 AND PERMISSION != ?2",
                           -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, DB_BANNED);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON_AddItemToArray(members, get_object_member(stmt));
    }
    sqlite3_finalize(stmt);
    return members;
}

void insert_member_into_db(sqlite3 *db,
                           gint room_id,
                           gint user_id,
                           gint8 permission) {
    gint32 rv = SQLITE_OK;
    sqlite3_str *sqlite_str = sqlite3_str_new(db);
    gchar *request = NULL;

    sqlite3_str_appendf(sqlite_str, "INSERT INTO MEMBERS(USER_ID, ROOM_ID, "
                                    "DATE, PERMISSION)VALUES(%d, %lu, "
                                    "%llu, %d);",
                        user_id, room_id, vm_get_time(DB_MILISECOND),
                        permission);
    request = sqlite3_str_finish(sqlite_str);
    rv = sqlite3_exec(db, request, 0, 0, 0);
    sqlite3_free(request);
}

/*
 * Function: insert_room_into_db
 * -------------------------------
 * makes an entry in the database in the rooms table
 * in which enters data from the structure t_db_room
 * 
 * room: structure t_db_room with filled fields except room_id and date
 * 
 * return: complements the structure of t_db_room
 */
static void sqlite_bind_room(sqlite3_stmt *stmt, t_db_room *room) {
    sqlite3_bind_text(stmt, 1, room->room_name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, room->user_id);
    sqlite3_bind_int64(stmt, 3, room->date);
    sqlite3_bind_text(stmt, 4, room->desc, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, room->type);
}

void insert_room_into_db(sqlite3 *db, t_db_room *room) {
    sqlite3_stmt *stmt;
    gint32 rv = SQLITE_OK;

    room->date = vm_get_time(DB_MILISECOND);
    rv = sqlite3_prepare_v2(db, "INSERT INTO ROOMS(NAME, USER_ID, DATE, "
                                "DESC, TYPE) VALUES(?1, ?2, ?3, ?4, ?5);",
                            -1, &stmt, 0);
    sqlite_bind_room(stmt, room);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    rv = sqlite3_prepare_v2(db, "SELECT ID FROM ROOMS WHERE DATE = ?1", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, room->date);
    sqlite3_step(stmt);
    room->room_id = sqlite3_column_int64(stmt, 0);
    insert_member_into_db(db, room->room_id, room->user_id, DB_ORDINARY);
    sqlite3_finalize(stmt);
}

void insert_auth_token(sqlite3 *db, gchar *login, gchar *auth_token) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "UPDATE USERS SET AUTH_TOKEN = ?1 WHERE LOGIN = ?2", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, auth_token, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, login, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool correct_credentials(sqlite3 *db, char *username, char *password) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE LOGIN = ?1 AND PASS = ?2", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

void create_new_user(sqlite3 *db, t_db_user *user) {
    sqlite3_stmt *stmt;

    user->date = vm_get_time(DB_MILISECOND);

    sqlite3_prepare_v2(db, "INSERT INTO USERS(NAME, LOGIN, PASS, AUTH_TOKEN, DATE, DESC)"
                           "VALUES (?1, ?2, ?3, ?4, ?5, ?6);", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, user->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user->pass, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user->auth_token, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 5, user->date);
    sqlite3_bind_text(stmt, 6, user->desc, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    get_user_id(db, user);
}

bool user_exists(sqlite3 *db, char *username) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE LOGIN = ?1", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

void create_messages_table(sqlite3 *db) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS MESSAGES("
                     "MESSAGE_ID    INTEGER PRIMARY KEY NOT NULL,"
                     "USER_ID       INTEGER             NOT NULL,"
                     "ROOM_ID       INTEGER             NOT NULL,"  
                     "DATE          INTEGER             NOT NULL,"
                     "MESSAGE       TEXT                        ,"
                     "TYPE          INTEGER             NOT NULL,"
                     "FILE_SIZE     INTEGER                     ,"
                     "FILE_NAME     TEXT                        ,"
                     "STATUS        INTEGER             NOT NULL);",
                     0, 0, 0);
}

void create_members_table(sqlite3 *db) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS MEMBERS("
                     "USER_ID        INTEGER NOT NULL,"
                     "ROOM_ID        INTEGER NOT NULL,"
                     "DATE           INTEGER NOT NULL,"
                     "PERMISSION     INTEGER NOT NULL);",
                 0, 0, 0);
}

void create_rooms_table(sqlite3 *db) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ROOMS("
                     "ID                 INTEGER PRIMARY KEY NOT NULL,"
                     "NAME               TEXT                NOT NULL,"
                     "USER_ID            INTEGER             NOT NULL,"
                     "DATE               INT                 NOT NULL,"
                     "DESC               TEXT                        ,"
                     "TYPE               INT                 NOT NULL);",
                     0, 0, 0);
}

void create_users_table(sqlite3 *db) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS USERS("
                     "ID             INTEGER PRIMARY KEY NOT NULL,"
                     "NAME           TEXT                NOT NULL,"
                     "LOGIN          TEXT  UNIQUE        NOT NULL,"
                     "PASS           TEXT                NOT NULL,"
                     "AUTH_TOKEN     TEXT                NOT NULL,"
                     "DATE           INTEGER             NOT NULL,"
                     "DESC           TEXT                );",
                 0, 0, 0);
}

void create_tables(sqlite3 *db) {
    create_users_table(db);
    create_rooms_table(db);
    create_members_table(db);
    create_messages_table(db);
}

sqlite3 *open_db() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        vm_logger(VM_LOG_FILE, "error: cannot open the database");
    } else {
        vm_logger(VM_LOG_FILE, "database has been opened successfully");
    }
    create_tables(db);
    return db;
}
