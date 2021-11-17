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

//transfer to our protocol lib
#define MX_J_STR(m) (m) ? (m) : "(null)"


/* REQUEST_TYPE
 * ----------
 * All types of request in protocol
 */
typedef enum e_request_type {
    RQ_SIGN_UP = 0,
    RQ_SIGN_IN,
    RQ_ADD_ROOM,
    RQ_COUNT
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


