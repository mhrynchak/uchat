#include "client.h"

static void file_read(gsize size, gchar *name, GInputStream *in) {
    gchar *basename = g_path_get_basename(name);
    gchar *filename = g_strjoin("", VM_FILES_DIR, basename, NULL);

    vm_file_read(size, filename, in);
    g_free(filename);
}

static void file_ready(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GInputStream *in_s = g_io_stream_get_input_stream(G_IO_STREAM(user_data));
    gchar *msg = g_data_input_stream_read_line_finish(G_DATA_INPUT_STREAM(source_object), res, NULL, NULL);
    cJSON *response = cJSON_Parse(msg);
    cJSON *size = NULL;
    cJSON *name = NULL;

    if (!response) {
        return;
    }
    size = cJSON_GetObjectItemCaseSensitive(response, "size");
    name = cJSON_GetObjectItemCaseSensitive(response, "name");
    if (!cJSON_IsNumber(size) || !cJSON_IsString(name)) {
        cJSON_Delete(response);
        return;
    }
    send_ready(user_data);
    file_read(size->valuedouble, name->valuestring, in_s);
    g_io_stream_close(G_IO_STREAM(user_data), NULL, NULL);
}

/*
 * Function: download_file_request
 * -------------------------------
 * Downloads file from the server
 * 
 * room_id: room id
 * msg_id: message id that contains file
 * chat: information about chat
 */
void download_file_request(gint room_id, gint msg_id, t_chat *chat) {
    GSocketConnection *conn = g_socket_client_connect_to_host(
        chat->client_conn, chat->argv[1],
        g_ascii_strtoll(chat->argv[2], NULL, 10), NULL, NULL);
    GInputStream *in_s = g_io_stream_get_input_stream(G_IO_STREAM(conn));
    GDataInputStream *in = g_data_input_stream_new(in_s);
    GOutputStream *out = g_io_stream_get_output_stream(G_IO_STREAM(conn));
    GDataOutputStream *out_d = g_data_output_stream_new(out);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;
    
    cJSON_AddNumberToObject(j_request, "token", RQ_DOWNLOAD_FILE);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    cJSON_AddNumberToObject(j_request, "msg_id", msg_id);
    cJSON_AddStringToObject(j_request, "auth_token", chat->auth_token);

    j_data = vm_message_calibration(j_request);
    vm_send(out_d, j_data);
    
    cJSON_Delete(j_request);
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, file_ready, conn);
}
