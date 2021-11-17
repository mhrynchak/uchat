#include "client.h"

///start forming requests through event handler
///and parsing responses through the json_manager
static void run_the_client(t_chat *chat) {
    vm_logger(VM_LOG_FILE, "client started");
    init_gui(chat);
    vm_logger(VM_LOG_FILE, "gui launched");
    start_gui(chat);
}

/* Connect to the server & Socket settings */ 
/* Init chat */
static void create_the_connection(t_chat *chat, int argc, char **argv) {
    GSocketConnection *connection = NULL;
    GSocketClient *client = g_socket_client_new();

    connection = g_socket_client_connect_to_host(client, argv[1], g_ascii_strtoll(argv[2], NULL, 10), NULL, NULL);
    vm_check_for_null(connection, "connection error: Invalid port or ip\n");

    init_chat(chat, client, connection, argc, argv);
}

int main(int argc, char **argv) {
    t_chat *chat = (t_chat *)malloc(sizeof(t_chat));

    valid_argv(argc, argv);
    vm_change_working_dir(CACHE);
    create_the_connection(chat, argc, argv);
    run_the_client(chat);

    return EXIT_SUCCESS;
}
