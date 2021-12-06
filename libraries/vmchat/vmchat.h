#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <libintl.h>
#include <gio/gio.h>
#include <glib.h>
#include "cJSON.h"


#define VM_LOG_FILE "info.log"
#define VM_HASH_REGEX "^[A-Fa-f0-9]{64}$"
#define VM_LOGIN_REGEX "^[a-z0-9_-]{3,22}$"

#define VM_MAX_FILE_SIZE 10000000
#define VM_FILES_DIR "files/"
#define VM_FILE_DELIM "__"

//transfer to our protocol lib
#define VM_J_STR(m) (m) ? (m) : "(null)"
#define MAX_MESSAGE 800
#define MAX_DESC 40
#define MAX_NAME 20
#define VM_TIME_SHORT 0
#define VM_TIME_LONG 1
#define VM_MAX_ROOM_NAME 100

/* MESSAGE_STATUS
 * ----------
 * All statuses of message
 */
typedef enum e_message_status {
    DB_MSG_START = 0,
    DB_MSG_EDIT,
}            t_message_status;

/* MESSAGE_TYPE
 * ----------
 * All types of message
 */
typedef enum e_message_type {
    DB_TEXT_MSG = 0,
    DB_FILE_MSG,
    DB_FILE_TEXT_MSG,
    DB_STICKER
}            t_message_type;

/* ROOM_TYPE
 * ----------
 * All types of room
 */
typedef enum e_room_type {
    DB_PRIVAT_CHAT = 0,
    DB_GLOBAL_CHAT,
    DB_LS_CHAT
}            t_room_type;

/* ERROR_TYPE
 * ----------
 * All types of errors in protocol
 */
typedef enum e_error_type {
    ER_AUTH_DATA = 0,
    ER_USER_EXST,
    ER_CLI_EXST,
    ER_COUNT
}            t_error_type;

typedef enum e_type_db {
    DB_NEW_MESSAGE = 0,
    DB_OLD_MESSAGE,
    DB_SECOND,
    DB_MILISECOND,
    DB_MICROSECOND
}            t_type_db;

/* MEMBER_TYPE
 * ----------
 * All types of member
 */
typedef enum e_member_type {
    DB_ORDINARY = 0,
    DB_ADMIN,
    DB_MODERATOR,
    DB_BANNED
}            t_member_type;

/* REQUEST_TYPE
 * ----------
 * All types of request in protocol
 */
typedef enum e_request_type {
    RQ_SIGN_UP = 0,
    RQ_SIGN_IN,
    RQ_TOKEN,
    RQ_MSG,
    RQ_EDIT_MSG,
    RQ_DEL_MSG,
    RQ_DEL_HIST,
    RQ_GET_NEW_MSGS,
    RQ_SEARCH_MSG,
    RQ_ERROR_MSG,
    RQ_OLD_MSGS,
    RQ_GET_ROOMS,
    RQ_NEW_ROOM,
    RQ_DEL_ROOM,
    RQ_JOIN_ROOM,
    RQ_UPD_ROOM_NAME,
    RQ_UPD_ROOM_DESC,
    RQ_UPD_ROOM_POWR,
    RQ_SEARCH_CH,
    RQ_GET_MEMBERS,
    RQ_MEMBER_INFO,
    RQ_NEW_MEMBER,
    RQ_BAN_MEMBER,
    RQ_UPD_USER,
    RQ_UPLOAD_FILE,
    RQ_DOWNLOAD_FILE,
    RQ_LOG_OUT,
    RQ_READY_READ,
    RQ_COUNT_REQUEST
}            t_request_type;

void vm_free(void **ptr);
void *vm_malloc(size_t size);
void vm_check(int res, char *msg);
void vm_change_working_dir(const char* dir);
void vm_logger(char *file_name, char *error);
void vm_check_for_null(void* res, char *msg);
gchar *vm_message_calibration(cJSON *j_data);
gboolean vm_is_connected(GSocketConnection *conn,
                         GDataOutputStream *out,
                         GDataInputStream *in);
gssize vm_send(GDataOutputStream *out_stream, gchar *str);
t_request_type vm_get_token(cJSON *j_object);
gchar *vm_get_valuestring(cJSON *j_object, gchar *key);
cJSON *vm_get_object(cJSON *j_object, gchar *key);
gchar *vm_get_time_in_str(guint64 miliseconds, gint8 format);
guint64 vm_get_time(gint8 type);
char *vm_strdup(char *str);
