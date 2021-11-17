//
// Created by Максим Гринчак on 11/4/21.
//
#include "client.h"

void json_manager(char *response, t_chat *chat) {
    cJSON *j_response = cJSON_Parse(response);
    t_request_type token = vm_get_token(j_response);
    
    switch (token) {
        case RQ_SIGN_UP: {
            sign_up_response(j_response, chat);
            break;
        }
        case RQ_SIGN_IN: {
            sign_in_response(j_response, chat);
            break;
        }
        case RQ_ADD_ROOM: {
            add_room_response(j_response, chat);
            break;
        }
        default: {
            vm_logger(VM_LOG_FILE, "invalid token");
            break;
        }
    }

    cJSON_Delete(j_response);
}
