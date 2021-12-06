#include "server.h"

static gboolean is_valid(GFile *file, gsize size, gsize bytes) {
    if (file && bytes != size) {
        if (g_file_query_exists(file, NULL)) {
            g_file_delete(file, NULL, NULL);
        }
        return false;
    }
    if (file && g_file_query_file_type(file, G_FILE_QUERY_INFO_NONE, NULL)
        != G_FILE_TYPE_REGULAR) {
        if (g_file_query_exists(file, NULL)) {
            g_file_delete(file, NULL, NULL);
        }
        return false;
    }
    return true;
}

static gboolean read_file(t_client *client, GFile *file,
                          gsize size, GFileOutputStream *out) {
    gsize bytes = 0;

    if (size <= VM_MAX_FILE_SIZE) {
        bytes = g_output_stream_splice(
            G_OUTPUT_STREAM(out), client->in_s,
            G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET,
            NULL, NULL);
    }
    return is_valid(file, size, bytes);
}

static void send_ready(t_client *client) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_READY_READ);
    j_data = vm_message_calibration(j_request);
    vm_send(client->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

gboolean read_file_req(t_client *client, gsize size, char *name) {
    GFile *file = g_file_new_for_path(name);
    GFileOutputStream *out = g_file_create(file, G_FILE_CREATE_NONE, NULL, NULL);

    send_ready(client);
    
    if (out == NULL || read_file(client, file, size, out) == false) {
        g_object_unref(file);
        return false;
    }
    g_object_unref(file);
    return true;
}
