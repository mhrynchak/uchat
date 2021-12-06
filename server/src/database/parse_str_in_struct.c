#include "server.h"

/*
 * Function: get_db_message
 * -------------------------------
 * writes data from the json object to the structure t_db_message
 * 
 * j_request: json object
 * 
 * return: filled structure t_db_message if all is well otherwise NULL
 */
t_db_message *get_db_message(cJSON *j_request) {
    t_db_message *message = vm_malloc(sizeof(t_db_message));

    message->room_id = vm_get_object(j_request, "room_id")->valueint;
    message->type = vm_get_object(j_request, "msg_type")->valueint;
    message->file_size = 0;
    message->message = g_strstrip(vm_get_valuestring(j_request, "msg"));
    message->file_name = g_strdup("");
    return message;
}

/*
 * Function: parse_json_user
 * -------------------------------
 * writes data from the json object to the structure t_db_user
 * 
 * j_request: json object
 * 
 * return: filled structure t_db_user if all is well otherwise NULL
 */
t_db_user *get_db_user(cJSON *j_request) {
    t_db_user *user = vm_malloc(sizeof(t_db_user));

    user->name = vm_get_valuestring(j_request, "name");
    user->login = vm_get_valuestring(j_request, "login");
    user->pass = vm_get_valuestring(j_request, "pass");
    user->desc = vm_get_valuestring(j_request, "desc");
    user->auth_token = "";
    return user;
}

/*
 * Function: get_db_room
 * -------------------------------
 * writes data from the json object to the structure t_db_room
 * 
 * room_j: json object
 * 
 * return: filled structure t_db_room if all is well otherwise NULL
 */
t_db_room *get_db_room(cJSON *j_request) {
    t_db_room *room = vm_malloc(sizeof(t_db_room));

    room->room_name = vm_get_valuestring(j_request, "name");
    room->desc = vm_get_valuestring(j_request, "desc");
    room->type = vm_get_object(j_request, "type")->valueint;
    return room;
}
