#include "client.h"

static gboolean is_server_ready(GSocketConnection *conn) {
    GInputStream *in = g_io_stream_get_input_stream(G_IO_STREAM(conn));
    GDataInputStream *in_d = g_data_input_stream_new(in);
    gchar *response = g_data_input_stream_read_line(in_d, NULL, NULL, NULL);
    t_request_type token = -1;
    cJSON *j_response = NULL;

    if (response) {
        j_response = cJSON_Parse(response);
        token = vm_get_token(j_response);
        if (token == RQ_READY_READ) {
            return TRUE;
        }
    }
    return FALSE;
}

static void send_file(gchar *j_data, GFile *file, t_chat *chat) {
    GSocketConnection *conn = g_socket_client_connect_to_host(
        chat->client_conn, chat->argv[1],
        g_ascii_strtoll(chat->argv[2], NULL, 10), NULL, NULL);
    GOutputStream *out = g_io_stream_get_output_stream(G_IO_STREAM(conn));
    GDataOutputStream *d_out = g_data_output_stream_new(G_OUTPUT_STREAM(out));
    GFileInputStream *file_in = g_file_read(file, NULL, NULL);

    vm_send(d_out, j_data);
    g_free(j_data);
    if (is_server_ready(conn)) {
        g_output_stream_splice(out, G_INPUT_STREAM(file_in),
        G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE, NULL, NULL);
    }
    g_io_stream_close(G_IO_STREAM(conn), NULL, NULL);
}

/*
 * Function: upload_file_request
 * -------------------------------
 * Uploads file to server
 * 
 * path: path to file
 * room_id: room id
 * chat: information about chat
 */
void upload_file_request(gchar *path, gint room_id, t_chat *chat) {
    GFile *file = g_file_new_for_path(path);
    GFileInfo *info = g_file_query_info(file, "standard::size,standard::name", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_UPLOAD_FILE);
    cJSON_AddStringToObject(j_request, "name", VM_J_STR(g_file_info_get_name(info)));
    cJSON_AddNumberToObject(j_request, "size", g_file_info_get_size(info));
    cJSON_AddStringToObject(j_request, "auth_token", VM_J_STR(chat->auth_token));
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    j_data = vm_message_calibration(j_request);

    if (g_file_info_get_size(info) <= VM_MAX_FILE_SIZE) {
        send_file(j_data, file, chat);
    }
    g_object_unref(info);
}
