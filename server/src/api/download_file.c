#include "server.h"

static void download(t_client *client, GFile *file, GDataInputStream *in_d) {
    GOutputStream *out = g_io_stream_get_output_stream(G_IO_STREAM(client->conn));
    GFileInputStream *in = g_file_read(file, NULL, NULL);
    gchar *ready = g_data_input_stream_read_line(in_d, NULL, NULL, NULL);
    cJSON *j_request = cJSON_Parse(ready);
    gint token = vm_get_token(j_request);

    if (token == RQ_READY_READ) {
        g_output_stream_splice( out, G_INPUT_STREAM(in),
        G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE, NULL, NULL);
    }
    if (ready) {
        g_free(ready);
    }
    cJSON_Delete(j_request);
    g_io_stream_close(G_IO_STREAM(client->conn), NULL, NULL);
}

void download_file_req(gchar *msg, t_client *client) {
    cJSON *j_response = cJSON_CreateObject();
    GFileInfo *info = NULL;
    gchar *j_data = NULL;
    GFile *file = NULL;

    if (msg == NULL) {
        return;
    }

    file = g_file_new_for_path(msg);
    info = g_file_query_info(file, "standard::size,standard::name", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if (msg && g_file_query_exists(file, NULL)) {
        cJSON_AddNumberToObject(j_response, "token", RQ_DOWNLOAD_FILE);
        cJSON_AddNumberToObject(j_response, "size", g_file_info_get_size(info));
        cJSON_AddStringToObject(j_response, "name", msg);
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);
        download(client, file, client->in);
        cJSON_Delete(j_response);
        g_free(j_data);
    }
    else {
        g_io_stream_close(G_IO_STREAM(client->conn), NULL, NULL);
    }
    g_object_unref(file);
    g_free(msg);
}
