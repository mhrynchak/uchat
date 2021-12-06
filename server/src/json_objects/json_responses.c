#include "server.h"

void clear_room_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (get_member_type(client->info->database, client->user->user_id, room_id) != DB_ORDINARY ||
        is_member(client->info->database, client->user->user_id, room_id) == false) {
        return;
    }

    delete_all_msgs(client->info->database, room_id);
    cJSON_AddNumberToObject(j_request, "token", RQ_DEL_HIST);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    j_data = vm_message_calibration(j_request);
    vm_send_to_all(j_data, client, room_id);

    g_free(j_data);
    cJSON_Delete(j_response);
}

void edit_room_desc_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gchar *room_desc = vm_get_valuestring(j_request, "desc");
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;
    
    if (get_member_type(client->info->database, client->user->user_id, room_id)
        != DB_ORDINARY || is_valid_info(room_desc, MAX_DESC) == false) {
        return;
    }

    edit_room_desc_by_id(client->info->database, room_id, room_desc);
    cJSON_AddNumberToObject(j_response, "token", RQ_UPD_ROOM_DESC);
    cJSON_AddNumberToObject(j_response, "room_id", room_id);
    cJSON_AddStringToObject(j_response, "desc", room_desc);
    j_data = vm_message_calibration(j_response);
    vm_send_to_all(j_data, client, room_id);

    g_free(j_data);
    g_free(room_desc);
    cJSON_Delete(j_response);
}

void edit_room_name_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gchar *room_name = vm_get_valuestring(j_request, "room_name");
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (get_member_type(client->info->database, client->user->user_id, room_id)
        != DB_ORDINARY || is_valid_room_name(room_name) == false) {
        return;
    }

    edit_room_name_by_id(client->info->database, room_id, room_name);
    cJSON_AddNumberToObject(j_response, "token", RQ_UPD_ROOM_NAME);
    cJSON_AddNumberToObject(j_response, "room_id", room_id);
    cJSON_AddStringToObject(j_response, "room_name", room_name);
    j_data = vm_message_calibration(j_response);
    vm_send_to_all(j_data, client, room_id);

    g_free(j_data);
    g_free(room_name);
    cJSON_Delete(j_response);
}

void get_member_info_response(cJSON *j_request, t_client *client) {
    gint user_id = vm_get_object(j_request, "user_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    t_db_user *user = NULL;
    gchar *j_data = NULL;

    user = get_user_by_id(client->info->database, user_id);
    cJSON_AddNumberToObject(j_response, "token", RQ_MEMBER_INFO);
    cJSON_AddStringToObject(j_response, "desc", user->desc);
    cJSON_AddStringToObject(j_response, "name", user->name);
    cJSON_AddStringToObject(j_response, "login", user->login);
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_response);
}

void edit_user_response(cJSON *j_request, t_client *client) {
    gchar *new_desc = vm_get_valuestring(j_request, "desc");
    gchar *new_name = vm_get_valuestring(j_request, "name");
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;
    
    if (is_valid_info(new_desc, MAX_DESC) == false || is_valid_info(new_name, MAX_NAME) == false) {
        return;
    }

    edit_user_name(client->info->database, client->user->user_id, new_name);
    edit_user_desc(client->info->database, client->user->user_id, new_desc);
    cJSON_AddNumberToObject(j_response, "token", RQ_UPD_USER);
    cJSON_AddStringToObject(j_response, "desc", new_desc);
    cJSON_AddStringToObject(j_response, "name", new_name);
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_free(j_data);
    g_free(new_desc);
    cJSON_Delete(j_response);
}

void del_msg_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gint msg_id = vm_get_object(j_request, "msg_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_member(client->info->database, client->user->user_id, room_id) &&
        get_member_type(client->info->database, client->user->user_id, room_id) == DB_ORDINARY &&
        is_msg_owner(client->info->database, client->user->user_id, msg_id)) {
        
        delete_message_by_id(client->info->database, msg_id);
        cJSON_AddNumberToObject(j_response, "token", RQ_DEL_MSG);
        cJSON_AddNumberToObject(j_response, "room_id", room_id);
        cJSON_AddNumberToObject(j_response, "msg_id", msg_id);
        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, client, room_id);
        g_free(j_data);
    }

    cJSON_Delete(j_response);
}

void del_room_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (get_member_type(client->info->database, client->user->user_id, room_id) == DB_ORDINARY) {
        cJSON_AddNumberToObject(j_response, "token", RQ_DEL_ROOM);
        cJSON_AddNumberToObject(j_response, "room_id", room_id);
        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, client, room_id);
        
        delete_room_by_id(client->info->database, room_id);
        g_free(j_data);
    }
    cJSON_Delete(j_response);
}

void ban_member_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gint user_id = vm_get_object(j_request, "user_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    edit_member_type(client->info->database, room_id, user_id, DB_BANNED);
    t_db_user *member = get_user_by_id(client->info->database, user_id);
    GDataOutputStream *out = g_hash_table_lookup(client->info->users, member->login);
    
    cJSON_AddNumberToObject(j_response, "token", RQ_DEL_ROOM);
    cJSON_AddNumberToObject(j_response, "room_id", room_id);
    j_data = vm_message_calibration(j_response);
    vm_send(out, j_data);
    
    cJSON_Delete(j_response);
    g_free(j_data);

    j_response = cJSON_CreateObject();
    cJSON_AddNumberToObject(j_response, "token", RQ_BAN_MEMBER);
    cJSON_AddNumberToObject(j_response, "room_id", room_id);
    cJSON_AddNumberToObject(j_response, "user_id", user_id);
    j_data = vm_message_calibration(j_response);
    vm_send_to_all(j_data, client, room_id);
    
    cJSON_Delete(j_response);
    g_free(j_data);
}

static bool is_valid_member(sqlite3 *db, gint user_id, gint room_id) {
    return get_member_type(db, user_id, room_id) != DB_BANNED && is_member(db, user_id, room_id);
}

static bool is_valid_msg_rq(gchar *msg, sqlite3 *db, gint user_id, gint room_id) {
    return is_valid_member(db, user_id, room_id) && is_valid_info(msg, MAX_MESSAGE);
}

void download_file_response(cJSON *j_request, t_client *client) {
    gchar *auth_token = vm_get_valuestring(j_request, "auth_token");
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gint msg_id = vm_get_object(j_request, "msg_id")->valueint;
    gchar *msg = get_message_by_id(client->info->database, msg_id);

    client->is_file = true;
    client->user = get_user_by_token(client->info->database, auth_token);
    if (is_valid_member(client->info->database, client->user->user_id, room_id)) {
        download_file_req(msg, client);
    }

    g_free(auth_token);
}

static gpointer upload_file_thread(gpointer data) {
    t_file_helper *file = (t_file_helper*)data;
    cJSON *j_response = cJSON_CreateObject();
    t_db_message *msg = NULL;
    gchar *filename = NULL;
    gchar *j_data = NULL;

    filename = g_strdup_printf("%s%llu%s%s%s",
    VM_FILES_DIR, vm_get_time(DB_MICROSECOND), file->client->user->login,
    VM_FILE_DELIM, file->name);

    if (read_file_req(file->client, file->size, filename) == true) {
        cJSON_AddNumberToObject(j_response, "room_id", file->room_id);
        cJSON_AddStringToObject(j_response, "msg", filename);
        cJSON_AddNumberToObject(j_response, "msg_type", DB_FILE_MSG);
        msg = get_db_message(j_response);
        msg->user_id = file->client->user->user_id;
        insert_message_into_db(file->client->info->database, msg);
        cJSON_Delete(j_response);

        j_response = cJSON_CreateObject();
        cJSON_AddNumberToObject(j_response, "token", RQ_MSG);
        cJSON_AddStringToObject(j_response, "msg", VM_J_STR(msg->message));
        cJSON_AddNumberToObject(j_response, "room_id", msg->room_id);
        cJSON_AddNumberToObject(j_response, "date", msg->date);
        cJSON_AddNumberToObject(j_response, "msg_id", msg->message_id);
        cJSON_AddNumberToObject(j_response, "user_id", msg->user_id);
        cJSON_AddNumberToObject(j_response, "msg_type", msg->type);
        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, file->client, file->room_id);

        g_free(j_data);
        cJSON_Delete(j_response);
    }
    g_io_stream_close(G_IO_STREAM(file->client->conn), NULL, NULL);
    file->client->upload_file = FALSE;
    g_free(filename);
    g_free(file->name);
    g_free(file);
    g_thread_exit(NULL);
    return NULL;
}

void upload_file_response(cJSON *j_request, t_client *client) {
    gchar *name = vm_get_valuestring(j_request, "name");
    gint size = vm_get_object(j_request, "size")->valueint;
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gchar *auth_token = vm_get_valuestring(j_request, "auth_token");
    t_file_helper *file = g_malloc0(sizeof(t_file_helper));
    
    client->is_file = true;
    client->user = get_user_by_token(client->info->database, auth_token);

    if (is_valid_member(client->info->database, client->user->user_id, room_id) == false) {
        return;
    }

    file->size = size;
    file->room_id = room_id;
    file->client = client;
    file->name = g_strdup(name);
    client->upload_file = true;
    g_thread_new("upload_thread", upload_file_thread, file);
    g_free(auth_token);
    g_free(name);
}

void join_to_room_response(cJSON *j_request, t_client *client) {
    gint room_id = vm_get_object(j_request, "id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    t_db_room *db_room = NULL;
    gchar *j_data = NULL;

    if (is_member(client->info->database, client->user->user_id, room_id) == false) {
        insert_member_into_db(client->info->database, room_id,
                              client->user->user_id, DB_ORDINARY);
        ///send response to the client that sent the request
        db_room = get_room_by_id(client->info->database, room_id);
        cJSON_AddNumberToObject(j_response, "token", RQ_JOIN_ROOM);
        cJSON_AddStringToObject(j_response, "name", VM_J_STR(db_room->room_name));
        cJSON_AddNumberToObject(j_response, "user_id", db_room->user_id);
        cJSON_AddNumberToObject(j_response, "id", db_room->room_id);
        cJSON_AddNumberToObject(j_response, "date", db_room->date);
        cJSON_AddStringToObject(j_response, "desc", VM_J_STR(db_room->desc));
        cJSON_AddStringToObject(j_response, "valid", "true");
        cJSON_AddStringToObject(j_response, "err_msg", "valid roomname");
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);
        cJSON_Delete(j_response);
        g_free(j_data);
        ///send response to all of the clients
        j_response = cJSON_CreateObject();
        cJSON_AddNumberToObject(j_response, "token", RQ_NEW_MEMBER);
        cJSON_AddNumberToObject(j_response, "user_id", client->user->user_id);
        cJSON_AddNumberToObject(j_response, "room_id", room_id);
        cJSON_AddStringToObject(j_response, "login", client->user->login);
        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, client, room_id);
        g_free(j_data);
        //free room
    }
    cJSON_Delete(j_response);
}

void search_msg_response(cJSON *j_request, t_client *client) {
    gchar *search_msg = vm_get_valuestring(j_request, "search_msg");
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    cJSON *j_msgs = NULL;
    gchar *j_data = NULL;

    if (is_valid_msg_rq(search_msg, client->info->database, client->user->user_id, room_id)) {
        j_msgs = search_msgs_in_db(client->info->database,
                                   client->user->user_id,
                                   search_msg);
        cJSON_AddNumberToObject(j_response, "token", RQ_SEARCH_MSG);
        cJSON_AddItemReferenceToObject(j_response, "msgs", j_msgs);
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);

        g_free(j_data);
    }

    g_free(search_msg);
    cJSON_Delete(j_response);
}

void search_room_response(cJSON *j_request, t_client *client) {
    cJSON *j_response = cJSON_CreateObject();
    cJSON *j_rooms = NULL;
    gchar *search_name = vm_get_valuestring(j_request, "search_name");
    gchar *j_data = NULL;

    if (strlen(search_name) > 0 && strlen(search_name) <= MAX_ROOM_LEN) {
        j_rooms = search_rooms_in_db(client->info->database,
                                     client->user->user_id,
                                     search_name);
        cJSON_AddNumberToObject(j_response, "token", RQ_SEARCH_CH);
        cJSON_AddItemReferenceToObject(j_response, "rooms", j_rooms);
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);

        g_free(j_data);
    }

    g_free(search_name);
    cJSON_Delete(j_response);
}

void log_out_response(cJSON *j_request, t_client *client) {
    gchar *auth_token = vm_get_valuestring(j_request, "auth_token");
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (strcmp(client->user->auth_token, auth_token) != 0) {
        return;
    }

    cJSON_AddNumberToObject(j_response, "token", RQ_LOG_OUT);
    cJSON_AddStringToObject(j_response, "auth_token", VM_J_STR(auth_token));
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_hash_table_remove(client->info->users, client->user->login);
    cJSON_Delete(j_response);
    g_free(j_data);
}

void new_messages_response(cJSON *j_request, t_client *client) {
    cJSON *date = vm_get_object(j_request, "date");
    cJSON *count = vm_get_object(j_request, "count");
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    cJSON *j_response = NULL;
    gchar *j_data = NULL;

    if(is_member(client->info->database, client->user->user_id, room_id)) {
        j_response = get_new_messages_by_id(client->info->database, room_id,
                                            date->valueint, count->valueint);
        cJSON_AddNumberToObject(j_response, "token", RQ_GET_NEW_MSGS);
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);
        g_free(j_data);
    }
    
    cJSON_Delete(j_response);
}

void old_messages_response(cJSON *j_request, t_client *client) {
    cJSON *date = vm_get_object(j_request, "date");
    cJSON *count = vm_get_object(j_request, "count");
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    cJSON *j_response = NULL;
    gchar *j_data = NULL;

    if(is_member(client->info->database, client->user->user_id, room_id) && 
       get_member_type(client->info->database, client->user->user_id, room_id) != DB_BANNED) {
        
        j_response = get_old_messages_by_id(client->info->database, room_id,
                                            date->valueint, count->valueint);
        cJSON_AddNumberToObject(j_response, "token", RQ_OLD_MSGS);
        j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);
        g_free(j_data);
    }

    cJSON_Delete(j_response);
}

void edit_message_response(cJSON *j_request, t_client *client) {
    gchar *new_msg_str = vm_get_valuestring(j_request, "msg");
    gint room_id = vm_get_object(j_request, "room_id")->valueint;
    gdouble msg_id = vm_get_object(j_request, "msg_id")->valuedouble;
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_member(client->info->database, client->user->user_id, room_id) &&
        is_msg_owner(client->info->database, client->user->user_id, msg_id) &&
        get_member_type(client->info->database, client->user->user_id, room_id) != DB_BANNED) {
        
        edit_message_by_id(client->info->database, msg_id, new_msg_str);
        cJSON_AddNumberToObject(j_response, "token", RQ_EDIT_MSG);
        cJSON_AddNumberToObject(j_response, "room_id", room_id);
        cJSON_AddNumberToObject(j_response, "msg_id", msg_id);
        cJSON_AddStringToObject(j_response, "msg", new_msg_str);

        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, client, room_id);
        g_free(j_data);
    }

    cJSON_Delete(j_response);
}

void send_message_response(cJSON *j_request, t_client *client) {
    t_db_message *msg = get_db_message(j_request);
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_valid_msg_rq(msg->message, client->info->database, client->user->user_id, msg->room_id)) {
        msg->user_id = client->user->user_id;
        insert_message_into_db(client->info->database, msg);

        cJSON_AddNumberToObject(j_response, "token", RQ_MSG);
        cJSON_AddStringToObject(j_response, "msg", VM_J_STR(msg->message));
        cJSON_AddNumberToObject(j_response, "room_id", msg->room_id);
        cJSON_AddNumberToObject(j_response, "date", msg->date);
        cJSON_AddNumberToObject(j_response, "msg_id", msg->message_id);
        cJSON_AddNumberToObject(j_response, "user_id", msg->user_id);
        cJSON_AddNumberToObject(j_response, "msg_type", msg->type);

        j_data = vm_message_calibration(j_response);
        vm_send_to_all(j_data, client, msg->room_id);
        g_free(j_data);
    }

    cJSON_Delete(j_response);
}

void get_members_response(cJSON *j_request, t_client *client) {
    cJSON *room_id = vm_get_object(j_request, "room_id");
    cJSON *j_members = get_json_members(client->info->database, room_id->valuedouble);
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_response, "token", RQ_GET_MEMBERS);
    cJSON_AddItemReferenceToObject(j_response, "members", j_members);
    cJSON_AddNumberToObject(j_response, "room_id", room_id->valueint);
    
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);
    
    g_free(j_data);
    cJSON_Delete(j_response);
}

void new_room_response(cJSON *j_request, t_client *client) {
    t_db_room *room = get_db_room(j_request);
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_valid_room_name(room->room_name)) {
        room->user_id = client->user->user_id;
        insert_room_into_db(client->info->database, room);
        ///form a cJSON response
        cJSON_AddNumberToObject(j_response, "token", RQ_NEW_ROOM);
        cJSON_AddNumberToObject(j_response, "type", RQ_JOIN_ROOM);
        cJSON_AddStringToObject(j_response, "valid", "true");
        cJSON_AddStringToObject(j_response, "name", VM_J_STR(room->room_name));
        cJSON_AddNumberToObject(j_response, "user_id", room->user_id);
        cJSON_AddNumberToObject(j_response, "id", room->room_id);
        cJSON_AddNumberToObject(j_response, "date", room->date);
        cJSON_AddStringToObject(j_response, "desc", VM_J_STR(room->desc));
        cJSON_AddStringToObject(j_response, "err_msg", "valid roomname");
    } else {
        cJSON_AddNumberToObject(j_response, "token", RQ_NEW_ROOM);
        cJSON_AddStringToObject(j_response, "valid", "false");
        cJSON_AddStringToObject(j_response, "err_msg", "invalid roomname");
    }
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_free(j_data);
    free_room(&room);
    cJSON_Delete(j_response);
}

void get_rooms_response(cJSON *j_request, t_client *client) {
    gint date = vm_get_object(j_request, "date")->valueint;
    cJSON *j_response = cJSON_CreateObject();
    cJSON *j_rooms = NULL;
    gchar *j_data = NULL;

    j_rooms = get_json_rooms(client->info->database, date, client->user->user_id);
    cJSON_AddNumberToObject(j_response, "token", RQ_GET_ROOMS);
    cJSON_AddItemReferenceToObject(j_response, "rooms", j_rooms);
    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);
    
    g_free(j_data);
    cJSON_Delete(j_response);
}

void sign_in_response(cJSON *j_request, t_client *client) {
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;
    gchar *valid_status = NULL;
    gchar *err_msg = NULL;

    t_db_user *user = get_db_user(j_request);
    user->auth_token = create_user_token(user->login);
    client->user = user;
    insert_auth_token(client->info->database, user->login, user->auth_token);
    t_db_user *check = get_user_by_login(client->info->database, user->login);
    
    if (check == NULL) {
        valid_status = "false";
        err_msg = "user not found";
    }
    else if (g_strcmp0(check->pass, client->user->pass)) {
        valid_status = "false";
        err_msg = "invalid password";
    }
    else if (g_hash_table_lookup(client->info->users, check->login)) {
        valid_status = "false";
        err_msg = "user already online";
    }
    else {
        client->user = check;
        valid_status = "true";
        err_msg = "correct credentials";
        g_hash_table_insert(client->info->users, client->user->login, client->out);
    }

    cJSON_AddNumberToObject(j_response, "token", RQ_SIGN_IN);
    cJSON_AddStringToObject(j_response, "auth_token", VM_J_STR(client->user->auth_token));
    cJSON_AddStringToObject(j_response, "login", VM_J_STR(client->user->login));
    cJSON_AddStringToObject(j_response, "desc", VM_J_STR(client->user->desc));
    cJSON_AddStringToObject(j_response, "name", VM_J_STR(client->user->name));
    cJSON_AddStringToObject(j_response, "valid", valid_status);
    cJSON_AddStringToObject(j_response, "err_msg", err_msg);

    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_response);
}

void sign_up_response(cJSON *j_request, t_client *client) {
    t_db_user *user = get_db_user(j_request);
    cJSON *j_response = cJSON_CreateObject();

    if(user_exists(client->info->database, user->login)) {
        cJSON_AddNumberToObject(j_response, "token", RQ_SIGN_UP);
        cJSON_AddStringToObject(j_response, "valid", "false");
        cJSON_AddStringToObject(j_response, "err_msg", "user already exists");
        gchar *j_data = vm_message_calibration(j_response);
        vm_send(client->out, j_data);
        g_free(j_data);
        free_db_user(user);
    }
    else {
        client->user = user;
        create_new_user(client->info->database, user);
        sign_in_response(j_request, client);
    }

    cJSON_Delete(j_response);
}
