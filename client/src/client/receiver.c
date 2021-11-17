#include "client.h"

/*
 * Function: receiver
 * -------------------------------
 * Receives requests from the server
 * 
 * source_object: the object the asynchronous operation was started with
 * res: GAsyncResult
 * user_data: chat information
 */
void receiver(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GDataInputStream *in = G_DATA_INPUT_STREAM(source_object);
    gsize count = 0;
    t_chat *chat = (t_chat*) user_data;
    char *response = NULL;

    if (vm_is_connected(chat->conn, chat->out, in) == false) {
        vm_logger(VM_LOG_FILE, "client is not connected");
    }
    response = g_data_input_stream_read_line_finish(in, res, &count, NULL);
    
    if (response) {
        fprintf(stdout, "response: %s\n", cJSON_Print(cJSON_Parse(response))); //del
        json_manager(response, chat);
    }

    g_free(response);
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, receiver, chat);
}
