#pragma once

#if defined(__APPLE__)
#include <malloc/malloc.h>
#define VM_MALLOC_SIZE(x) malloc_size(x)
#elif defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#define VM_MALLOC_SIZE(x) _msize(x)
#elif defined(__linux__)
#include <malloc.h>
#define VM_MALLOC_SIZE(x) malloc_usable_size(x)
#define SO_REUSEPORT 15
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <sys/stat.h>
#include <gio/gio.h>
#include <glib.h>
#include "cJSON.h"
#include "sqlite3.h"
#include "database.h"
#include "vmchat.h"


#define MAX_CLIENTS 10
#define BUF_SIZE 2048
#define BACKLOG 10
#define NAME_LEN 32
#define IP_LEN 15
#define MAX_ROOM_LEN 100

#define CACHE "server/cache"
#define VM_REQUEST_PER_SECOND 20
#define VM_DELAY (1000000 / VM_REQUEST_PER_SECOND)

typedef struct s_send_helper {
    GHashTable *table;
    gchar *j_data;
}              t_send_helper;

typedef struct s_info {
    GHashTable *users;
    sqlite3* database;
}              t_info;

typedef struct s_client {
    GSocketConnection *conn;
    GDataOutputStream *out;
    GDataInputStream *in;
    GInputStream *in_s;
    gchar *request;
    t_db_user *user;
    t_info *info;
    gboolean upload_file;
    gboolean is_file;
}              t_client;

typedef struct s_file_helper {
    t_client *client;
    guint64 size;
    guint64 room_id;
    gchar *name;
}              t_file_helper;

///server
void uchat_daemon(void);
void run_the_server(char *port);


///**********************|server utils|**********************///
void vm_send_to_all(gchar *j_data, t_client *client, gint room_id);


///**********************|json_objects|**********************///
void json_manager(t_client *client);
///responses
void sign_up_response(cJSON *j_request, t_client *client);
void sign_in_response(cJSON *j_request, t_client *client);
void new_room_response(cJSON *j_request, t_client *client);
void get_rooms_response(cJSON *j_request, t_client *client);
void get_members_response(cJSON *j_request, t_client *client);
void send_message_response(cJSON *j_request, t_client *client);
void edit_message_response(cJSON *j_request, t_client *client);
void old_messages_response(cJSON *j_request, t_client *client);
void log_out_response(cJSON *j_request, t_client *client);
void search_room_response(cJSON *j_request, t_client *client);
void search_msg_response(cJSON *j_request, t_client *client);
void join_to_room_response(cJSON *j_request, t_client *client);
void new_messages_response(cJSON *j_request, t_client *client);
void new_member_response(cJSON *j_request, t_client *client);
void upload_file_response(cJSON *j_request, t_client *client);
void download_file_response(cJSON *j_request, t_client *client);
void ban_member_response(cJSON *j_request, t_client *client);
void del_room_response(cJSON *j_request, t_client *client);
void del_msg_response(cJSON *j_request, t_client *client);
void edit_user_response(cJSON *j_request, t_client *client);
void get_member_info_response(cJSON *j_request, t_client *client);
void edit_room_name_response(cJSON *j_request, t_client *client);
void edit_room_desc_response(cJSON *j_request, t_client *client);
void clear_room_response(cJSON *j_request, t_client *client);

////api
void get_user_id(sqlite3 *db, t_db_user *user);
gchar *create_user_token(char *login);
gboolean is_valid_room_name(gchar *roomname);
gboolean is_valid_info(gchar *msg, gsize max_value);
gint8 get_member_type(sqlite3 *db, gint user_id, gint room_id);
GList *get_members_list(sqlite3 *db, gint room_id);
gboolean read_file_req(t_client *client, gsize size, char *name);
void download_file_req(gchar *msg, t_client *client);

///utils
t_info *init_info(void);
void deinit_info(t_info **info);
void deinit_client(t_client **client);
void free_db_user(t_db_user *user);
