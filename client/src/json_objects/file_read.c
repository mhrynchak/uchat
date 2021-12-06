#include "client.h"
 
static gboolean is_valid(GFile *file, gsize bytes, gsize size) {
    if (bytes != size) {
        g_file_delete(file, NULL, NULL);
        return FALSE;
    }
    if (g_file_query_file_type(file, G_FILE_QUERY_INFO_NONE, NULL)
        != G_FILE_TYPE_REGULAR) {
        g_file_delete(file, NULL, NULL);
        return FALSE;
    }
    return TRUE;
}

static void file_read(gsize size, GFileOutputStream *out, GInputStream *in, GFile *file) {
    gsize bytes = 0;

    if (size <= VM_MAX_FILE_SIZE) {
        bytes = g_output_stream_splice(
            G_OUTPUT_STREAM(out), in, G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET,
            NULL, NULL);
    }
    if (!is_valid(file, bytes, size)) {
        return;
    }
}

void send_ready(GSocketConnection *conn) {
    GOutputStream *out = g_io_stream_get_output_stream(G_IO_STREAM(conn));
    GDataOutputStream *out_d = g_data_output_stream_new(out);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_READY_READ);
    j_data = vm_message_calibration(j_request);
    vm_send(out_d, j_data);
    
    cJSON_Delete(j_request);
}

void vm_file_read(gsize size, gchar *name, GInputStream *in) {
    GFile *file = g_file_new_for_path(name);
    GFileOutputStream *out = g_file_create(file, G_FILE_CREATE_NONE, NULL, NULL);

    if (!out) {
        return;
    }
    file_read(size, out, in, file);
    if (file) {
        g_object_unref(file);
    }
}
