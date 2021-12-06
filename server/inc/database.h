#pragma once

#include "server.h"

#define DB_PATH "database.db"

typedef struct s_member{
    gint user_id;
    gint room_id;
    gint date;
    gint8   prm;
}              t_member;

typedef struct s_db_message {
    gint user_id;
    gint room_id;
    gint message_id;
    gint date;
    gint8 status;
    gint8 type;
    gint file_size;
    gchar *message;
    gchar *file_name;
}              t_db_message;

typedef struct s_db_user {
    gint user_id;
    gint date;
    gchar *pass;
    gchar *auth_token;
    gchar *name;
    gchar *login;
    gchar *desc;
}             t_db_user;

typedef struct s_db_room {
    gchar *room_name;
    gint room_id;
    gint user_id;
    gint date;
    gint type;
    gchar *desc;
}              t_db_room;

cJSON *get_json_rooms(sqlite3 *db, guint64 date, guint64 user_id);
cJSON *get_json_members(sqlite3 *db, gint room_id);
void insert_room_into_db(sqlite3 *db, t_db_room *room);
void insert_auth_token(sqlite3 *db, char *username, char *token);
bool user_not_online(sqlite3 *db, char *username);
bool correct_credentials(sqlite3 *db, char *username, char *password);
void create_new_user(sqlite3 *db, t_db_user *user);
bool user_exists(sqlite3 *db, char *username);
sqlite3 *open_db();


t_db_message *get_db_message(cJSON *message_j);
t_db_user *get_db_user(cJSON *j_request);
t_db_room *get_db_room(cJSON *j_request);
void close_db(sqlite3 *db);

void free_message(t_db_message **message);
void free_room(t_db_room **room);
void free_user(t_db_user **user);

cJSON *get_old_messages_by_id(sqlite3 *db, gint room_id,
                              gint date, gint64 count);
gchar *create_request_message_by_id(sqlite3 *db, gint room_id, gint8 type);
gboolean is_msg_owner(sqlite3 *db, gint user_id, gint msg_id);
gboolean is_member(sqlite3 *db, gint user_id, gint room_id);
void edit_message_by_id(sqlite3 *db, gint id, gchar *new);
void edit_user_name_by_id(sqlite3 *db, gint id, gchar *new_name);
cJSON *search_rooms_in_db(sqlite3 *db, gint user_id, gchar *str_search);
cJSON *search_msgs_in_db(sqlite3 *db, gint user_id, gchar *str_search);
cJSON *get_object_message(sqlite3_stmt *stmt);
void insert_member_into_db(sqlite3 *db, gint room_id, gint user_id, gint8 permission);
t_db_room *get_room_by_id(sqlite3 *db, guint64 id);
cJSON *get_new_messages_by_id(sqlite3 *db, gint room_id, gint date, gint64 count);
void insert_message_into_db(sqlite3 *db, t_db_message *message);
t_db_user *get_user_by_token(sqlite3 *db, gchar *token);
t_db_user *get_user_by_login(sqlite3 *db, gchar *login);
gchar *get_message_by_id(sqlite3 *db, guint64 message_id);
t_db_user *get_user_by_id(sqlite3 *db, guint64 user_id);
void edit_member_type(sqlite3 *db, gint room_id, gint user_id, gint8 new_type);
void delete_room_by_id(sqlite3 *db, guint64 room_id);
void delete_message_by_id(sqlite3 *db, guint64 id);
void edit_user_desc(sqlite3 *db, guint64 user_id, gchar *new_desc);
void edit_user_name(sqlite3 *db, guint64 user_id, gchar *new_desc);
void edit_room_name_by_id(sqlite3 *db, gint id, gchar *new_name);
void edit_room_desc_by_id(sqlite3 *db, gint id, gchar *new_name);
void delete_message_by_id(sqlite3 *db, guint64 id);
void delete_all_msgs(sqlite3 *db, gint room_id);
