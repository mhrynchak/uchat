#pragma once

#if defined(__APPLE__)
#include <malloc/malloc.h>
#define MX_MALLOC_SIZE(x) malloc_size(x)
#elif defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#define MX_MALLOC_SIZE(x) _msize(x)
#elif defined(__linux__)
#include <malloc.h>
#define MX_MALLOC_SIZE(x) malloc_usable_size(x)
#define SO_REUSEPORT 15
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <sys/stat.h>
#include <gio/gio.h>
#include <glib.h>
#include "cJSON.h"
#include "sqlite3.h"
#include "database.h"
#include "vmchat.h"


#define MAX_CLIENTS 10
#define BUF_SIZE 2048
#define BACKLOG 10
#define NAME_LEN 32
#define IP_LEN 15
#define MAX_ROOM_LEN 100

#define CACHE "server/cache"
#define MX_REQUEST_PER_SECOND 20
#define MX_DELAY (1000000 / MX_REQUEST_PER_SECOND)

typedef struct {
    GSocketConnection *conn;
    GDataOutputStream *out;
    GDataInputStream *in;
    GInputStream *in_s;
    gchar *request;
    sqlite3 *db;
} t_client;

///server
void uchat_daemon(void);
void run_the_server(char *port);
gssize send_the_response(GDataOutputStream *out, char *response);


///**********************|json_objects|**********************///
void json_manager(t_client *client);
///responses
void sign_up_response(cJSON *j_request, t_client *client);
void sign_in_response(cJSON *j_request, t_client *client);
void add_room_response(cJSON *j_request, t_client *client);

////api
gboolean is_valid_room_name(gchar *roomname);
