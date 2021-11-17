#include "vmchat.h"

gchar *vm_get_valuestring(cJSON *j_object, gchar *key) {
    return strdup(cJSON_GetObjectItemCaseSensitive(j_object, key)->valuestring);
}

t_request_type vm_get_token(cJSON *j_object) {
    return atoi(cJSON_GetObjectItemCaseSensitive(j_object, "token")->valuestring);
}

/*
 * Function: sender
 * -------------------------------
 * Sends request to server
 * 
 * out: output stream to the server
 * str: data to be sent to the server
 * 
 * returns: count of sent bytes
 */

gssize vm_send(GDataOutputStream *out_stream, gchar *str) {
    GError *error = NULL;
    gsize size = 0;

    if (str) {
        if ((size = g_data_output_stream_put_string(out_stream, str, NULL, &error)) <= 0) {
            vm_logger(VM_LOG_FILE, "Write string error");
        }
        if (error) {
            vm_logger(VM_LOG_FILE, "Error send");
            g_clear_error(&error);
        }
    }
    return size;
}

/*
 * Function: vm_is_connected
 * -------------------------------
 * Checks for connection status of sockets and streams
 */
gboolean vm_is_connected(GSocketConnection *conn, 
                         GDataOutputStream *out,
                         GDataInputStream *in) {
    if (g_socket_connection_is_connected(conn) == false
        || g_output_stream_is_closed(G_OUTPUT_STREAM(out))
        || g_input_stream_is_closed(G_INPUT_STREAM(in))) {
        return false;
    }
    return true;
}

/*
 * Function: vm_message_calibration
 * -------------------------------
 * Calibrate cJSON object into a valid string message before sending
 */
gchar *vm_message_calibration(cJSON *j_data) {
    char *req_body = cJSON_PrintUnformatted(j_data);
    size_t req_len = strlen(req_body);
    size_t buf_size = req_len + 2;
    char str[buf_size];

    bzero(str, buf_size);
    strcpy(str, req_body);
    str[buf_size - 2] = '\n';
    return strdup(str);
}

/*
 * Function: vm_change_working_dir
 * -------------------------------
 * Changes working directory if its exist or creating it
 */
void vm_change_working_dir(const char *dir) {
    if (g_mkdir_with_parents(dir, 0775) < 0) {
        vm_logger(VM_LOG_FILE, "Error creating working directory");
        exit(EXIT_FAILURE);
    } 
    if (chdir(dir)) { 
        vm_logger(VM_LOG_FILE, "No working directory");
        exit(EXIT_FAILURE);
    }
}
/*
 * Function: vm_logger
 * -------------------------------
 * Logs string to file
 */
void vm_logger(char *file_name, char *error) {
    FILE *fd = fopen(file_name, "a");
    gchar *date = NULL;
    GDateTime *dt = g_date_time_new_now_local();
    date = g_date_time_format(dt, "%e.%m.%Y %T");
    fprintf(stderr, "log: %s\n", error);
    fprintf(fd, "d:%s  ", date);
    fprintf(fd, "pid:%d  ", getpid());
    fprintf(fd, "m:'%s'\n", error);
    fclose(fd);
}
/*
 * Function: vm_free
 * -------------------------------
 * Wrapper of standard function free
 */
void vm_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}
/*
 * Function: vm_check
 * -------------------------------
 * Wrapper of standard functions write and exit
 */
void vm_check(int res, char *msg) {
    if (res < 0) {
        if (write(STDERR_FILENO, msg, strlen(msg))) {
            exit(EXIT_FAILURE);
        }
    }
}
/*
 * Function: vm_check_for_null
 * -------------------------------
 * Wrapper of standard functions write and exit
 */
void vm_check_for_null(void* res, char *msg) {
    if (res == NULL) {
        if (write(STDERR_FILENO, msg, strlen(msg))) {
            exit(EXIT_FAILURE);
        }
    }
}
/*
 * Wrapper of standart function malloc
 */
void *vm_malloc(size_t size) {
    void *data = malloc(size);

    if (!data) {
        vm_logger(VM_LOG_FILE, "malloc failed");
    }
    return data;
}
