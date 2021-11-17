#include "client.h"

static void init_fields(t_chat *chat, gint argc, char **argv) {
    chat->auth_token = NULL;
    chat->valid = TRUE;
    chat->login = NULL;
    chat->builder = NULL;
    chat->data = NULL;
    chat->upl_old_msgs = FALSE;
    chat->msg_placeholder = TRUE;
    chat->shift_hold = FALSE;
    chat->css_prov = NULL;
    chat->argc = argc;
    chat->argv = argv;
}
/*
 * Function: init_chat
 * -------------------------------
 * Initializate chat information
 * 
 * connection: connection between server and client
 * argc: arguments count
 * argv: arguments array
 * returns: new chat information
 */
void init_chat(t_chat *chat, GSocketClient *client, GSocketConnection *connection, gint argc, char **argv) {
    init_fields(chat, argc, argv);

    GOutputStream *out_s = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataOutputStream *out = g_data_output_stream_new(out_s);
    GInputStream *in_s = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GDataInputStream *in = g_data_input_stream_new(in_s);

    chat->conn = connection;
    chat->client_conn = client;
    chat->out = out;
    chat->in = in;
#ifdef TEST_DEBUG
    chat->builder = NULL;
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, receiver, chat);
#else
    chat->builder = init_window(argc, argv);
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, receiver, chat);
    css_connect_from_file(chat);
#endif
}
