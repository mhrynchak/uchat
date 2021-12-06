#include "client.h"

void json_manager(gchar *response, t_chat *chat) {
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
        case RQ_LOG_OUT: {
            log_out_response(j_response, chat);
            break;
        }
        case RQ_GET_ROOMS: {
            get_rooms_response(j_response, chat);
            break;
        }
        case RQ_NEW_ROOM: {
            new_room_response(j_response, chat);
            break;
        }
        case RQ_GET_MEMBERS: {
            get_members_response(j_response, chat);
            break;
        }
        case RQ_MSG: {
            send_message_response(j_response, chat);
            break;
        }
        case RQ_EDIT_MSG: {
            edit_message_response(j_response, chat);
            break;
        }
        case RQ_OLD_MSGS: {
            old_messages_response(j_response, chat);
            break;
        }
        case RQ_GET_NEW_MSGS: {
            new_messages_response(j_response, chat);
            break;
        }
        case RQ_SEARCH_CH: {
            search_room_response(j_response, chat);
            break;
        }
        case RQ_SEARCH_MSG: {
            search_msg_response(j_response, chat);
            break;
        }
        case RQ_JOIN_ROOM: {
            join_to_room_response(j_response, chat);
            break;
        }
        case RQ_NEW_MEMBER: {
            new_member_response(j_response, chat);
            break;
        }
        case RQ_BAN_MEMBER: {
            ban_member_response(j_response, chat);
            break;
        }
        case RQ_DEL_ROOM: {
            del_room_response(j_response, chat);
            break;
        }
        case RQ_DEL_MSG: {
            del_msg_response(j_response, chat);
            break;
        }
        case RQ_UPD_USER: {
            edit_user_response(j_response, chat);
            break;
        }
        case RQ_MEMBER_INFO: {
            get_member_info_response(j_response, chat);
            break;
        }
        case RQ_UPD_ROOM_NAME: {
            edit_room_name_response(j_response, chat);
            break;
        }
        case RQ_UPD_ROOM_DESC: {
            edit_room_desc_response(j_response, chat);
            break;
        }
        case RQ_DEL_HIST: {
            clear_room_response(j_response, chat);
            break;
        }
        default: {
            vm_logger(VM_LOG_FILE, "invalid token");
            break;
        }
    }

    cJSON_Delete(j_response);
}
