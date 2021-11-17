#include "client.h"
#include <netdb.h>

static void err() {
    fprintf(stderr, "Usage: ./uchat <ip address> <port>\n"
                    "or:    ./uchat <hostname>:<port>\n");
    exit(EXIT_FAILURE);
}

static void check_port(char *port) {
    int i = -1;
    bool valid = true;
    if (strlen(port) != 4) {
        valid = false;
    }
    while (port[++i]) {
        if (!isdigit(port[i])) {
            valid = false;
            break;
        }
    }
    if (!valid || !atoi(port)) {
        err();
    }
}

static void check_ip(char *ip) {
    int dots = 0;
    bool valid = true;
    for (int i = 0; ip[i]; i++) {
        if (ip[i] == '.') {
            dots++;
        }
        if (ip[i] != '.' && !isdigit(ip[i])) {
            valid = false;
        }
        if ((ip[i] == '.' && ip[i + 1] == '.')
            || (ip[i] == '.' && !isdigit(ip[i + 1]))
            || (ip[i] == '.' && !isdigit(ip[i - 1]))) {
            valid = false;
        }
    }
    if (dots != 3 || valid == false) {
        err();
    }
}

/*
 * Function: valid_argv
 * -------------------------------
 * Validation and/or converting IP address and port
 */
void valid_argv(int argc, char **argv) {
    char buf[BUFSIZ];
    struct hostent * hp;
    char **tmp_arr;
    if (argc == 3 ) {
        check_ip(argv[1]);
        check_port(argv[2]);
    }
    else if (argc == 2) {
        tmp_arr = g_strsplit(argv[1], ":", -1);
        if(tmp_arr == NULL || tmp_arr[1] == NULL || tmp_arr[2] != NULL) {
            g_strfreev(tmp_arr);
            err();
        }

        argv[1] = strtok(argv[1],":");
        argv[2] = strdup(tmp_arr[1]);
        argc++;
        g_strfreev(tmp_arr);

        if (( hp = gethostbyname(argv[1])) == 0 ){
            fprintf(stderr, "Host <%s> not found!\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        if ( hp->h_addrtype != AF_INET ){
            fprintf(stderr, "Unknown address type!\n");
            exit(EXIT_FAILURE);
        }

        argv[1] = strdup((char*)inet_ntop(hp->h_addrtype, *(hp->h_addr_list), buf, sizeof(buf)));
        check_port(argv[2]);
    }
    else {
        err();
    }
}
