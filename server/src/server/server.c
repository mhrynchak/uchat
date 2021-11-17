#include "server.h"

/*
 * Function: handle_request
 * -------------------------------
 * Callback for ready message from client
 * 
 * source_object: the object the asynchronous operation was started with
 * res: GAsyncResult
 * user_data: client that sent message
 */
static void handle_request(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GDataInputStream *in = G_DATA_INPUT_STREAM(source_object);
    gsize count = 0;
    t_client *client = (t_client*)user_data;

    if (vm_is_connected(client->conn, client->out, client->in) == false) {
        vm_logger(VM_LOG_FILE, "socket is not connected");
        /// mx_deinit_client(&client);
        return;
    }

    client->request = g_data_input_stream_read_line_finish(in, res, &count, NULL);
    if(client->request) {
        fprintf(stdout, "request: %s\n", cJSON_Print(cJSON_Parse(client->request)));                ///del
        json_manager(client);
    }
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, handle_request, client);
}


/*
 * Function: incoming
 * -------------------------------
 * Callback for incoming connection
 * 
 * service: socket_service
 * conn: connection between server and client
 * souce_object: the source_object passed to g_socket_listener_add_address()
 * user_data: information about another users, database, handlers
 * 
 * returns: stop other handlers from being called
 */
static gboolean incoming(GSocketService *service,
                                  GSocketConnection *conn,
                                  GObject *source_object,
                                  gpointer arg_data) {
    GOutputStream *out_s = g_io_stream_get_output_stream(G_IO_STREAM(conn));
    GInputStream *in_s = g_io_stream_get_input_stream(G_IO_STREAM(conn));
    GDataOutputStream *out = g_data_output_stream_new(out_s);
    GDataInputStream *in = g_data_input_stream_new(in_s);
    t_client *client = vm_malloc(sizeof(t_client));

    client->out = out;
    client->db = (sqlite3*)arg_data;
    client->conn = g_object_ref(conn);
    client->in = in;
    client->in_s = in_s;
    g_data_input_stream_read_line_async(in, G_PRIORITY_DEFAULT, NULL, handle_request, client);
    (void)source_object;
    (void)service;
    return false;
}

static void valid_argv(gint argc, char *port) {
    if (argc != 2 || strlen(port) != 4 || !atoi(port)) {
        fprintf(stderr, "Usage: ./uchat_server <port>\n");
        exit(1);
    }
}


gint main(gint argc, char **argv) {
    GSocketService *service = g_socket_service_new();
    GMainLoop *loop = NULL;
    sqlite3 *database = NULL;
    gint64 port = -1;

    valid_argv(argc, argv[1]);
    vm_change_working_dir(CACHE);
    // uchat_daemon();
    database = open_db();
    port = g_ascii_strtoll(argv[1], NULL, 10);

    if (!g_socket_listener_add_inet_port((GSocketListener*)service, port, NULL, NULL)) {
        g_printerr("invalid port\n");
        return -1;
    }
    g_signal_connect(service, "incoming", G_CALLBACK(incoming), database);
    g_socket_service_start(service);
    vm_logger(VM_LOG_FILE, "uchat_server started");
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    return 0;
}
