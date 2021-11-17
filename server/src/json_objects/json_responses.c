//
// Created by Максим Гринчак on 11/4/21.
//
#include "server.h"

void add_room_response(cJSON *j_request, t_client *client) {
    gchar *roomname = vm_get_valuestring(j_request, "roomname");
    gchar *descrpition = vm_get_valuestring(j_request, "description");
    cJSON *j_response = cJSON_CreateObject();
    gchar *j_data = NULL;
    gchar *valid = NULL;
    gchar *err_msg = NULL;

    if (is_valid_room_name(roomname)) {
        insert_room_into_db(client->db, roomname);
        valid = "true";
        err_msg = "room creation successfull";
    }
    else {
        valid = "false";
        err_msg = "invalid roomname";
    }

    cJSON_AddNumberToObject(j_response, "token", RQ_ADD_ROOM);
    cJSON_AddStringToObject(j_response, "roomname", MX_J_STR(roomname));
    cJSON_AddStringToObject(j_response, "valid", MX_J_STR(valid));
    cJSON_AddStringToObject(j_response, "err_msg", MX_J_STR(err_msg));

    j_data = vm_message_calibration(j_response);
    vm_send(client->out, j_data);

    g_free(j_data);
    g_free(roomname);
    g_free(descrpition);
    cJSON_Delete(j_request);
    cJSON_Delete(j_response);
}

void sign_in_response(cJSON *j_request, t_client *client) {
    gchar *username = vm_get_valuestring(j_request, "username");
    gchar *password = vm_get_valuestring(j_request, "password");
    cJSON *j_response = cJSON_CreateObject();
    gboolean signed_in = false;
    gchar *j_data = NULL;
    gchar *valid_status = NULL;
    gchar *err_msg = NULL;

    if (correct_credentials(client->db, username, password)) {
        if (user_not_online(client->db, username)) {
            insert_socket(client->db, username, (int) client->conn);
            valid_status = "true";
            err_msg = "correct credentials";
            signed_in = true;
        }
        else {
            valid_status = "false";
            err_msg = "already online";
        }
    }
    else {
        valid_status = "false";
        err_msg = "invalid credentials";
    }

    cJSON_AddNumberToObject(j_response, "token", RQ_SIGN_IN);
    cJSON_AddStringToObject(j_response, "valid", valid_status);
    cJSON_AddStringToObject(j_response, "err_msg", err_msg);

    j_data = vm_message_calibration(j_response);
    // fprintf(stdout, "response: %s\n", cJSON_Print(cJSON_Parse(j_data)));
    vm_send(client->out, j_data);

    g_free(j_data);
    g_free(username);
    g_free(password);
    cJSON_Delete(j_request);
    cJSON_Delete(j_response);

    if (signed_in) {
        init_client();
    }
}

void sign_up_response(cJSON *j_request, t_client *client) {
    gchar *username = vm_get_valuestring(j_request, "username");
    gchar *password = vm_get_valuestring(j_request, "password");
    cJSON *j_response = cJSON_CreateObject();

    if(user_exists(client->db, username)) {
        cJSON_AddNumberToObject(j_response, "token", RQ_SIGN_UP);
        cJSON_AddStringToObject(j_response, "valid", "false");
        cJSON_AddStringToObject(j_response, "err_msg", "user already exists");
        gchar *j_data = vm_message_calibration(j_response);
        // fprintf(stdout, "response\n: %s\n", cJSON_Print(cJSON_Parse(j_data)));
        vm_send(client->out, j_data);
        g_free(j_data);
    }
    else {
        create_new_user(client->db, username, password);
        sign_in_response(j_request, client);
    }

    free(username);
    free(password);
    cJSON_Delete(j_request);
    cJSON_Delete(j_response);
}
