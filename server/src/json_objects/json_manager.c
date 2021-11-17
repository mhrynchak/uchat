//
// Created by Максим Гринчак on 11/4/21.
//
#include "server.h"

void json_manager(t_client *client) {
    cJSON *j_request = cJSON_Parse(client->request);
    t_request_type token = vm_get_token(j_request);

    switch (token) {
        case RQ_SIGN_UP: {
            sign_up_response(j_request, client);
            break;
        }
        case RQ_SIGN_IN: {
            sign_in_response(j_request, client);
            break;
        }
        case RQ_ADD_ROOM: {
            add_room_response(j_request, client);
            break;
        }
        default: {
            vm_logger(VM_LOG_FILE, "invalid token");
            break;
        }
    }

    cJSON_Delete(j_request);
    g_usleep(MX_DELAY);
}
