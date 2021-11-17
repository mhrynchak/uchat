#include "client.h"
#include <stdlib.h>

void add_room_request(t_chat *chat) {
    gchar *roomname = get_buffer_text("buffer_roomname", chat->builder);
    gchar *description = get_buffer_text("buffer_description", chat->builder);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_ADD_ROOM);
    cJSON_AddStringToObject(j_request, "roomname", MX_J_STR(roomname));
    cJSON_AddStringToObject(j_request, "description", MX_J_STR(description));
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);
    
    cJSON_Delete(j_request);
}

void sign_in_request(t_chat *chat) {
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_login");
    gchar *username = get_buffer_text("buffer_login", chat->builder);
    gchar *password = get_buffer_text("buffer_password", chat->builder);
    cJSON *j_request = cJSON_CreateObject();
    gchar *encrypted_pass = encrypt_str(password);
    gchar *j_data = NULL;

    if (is_valid_auth_data(username, password, GTK_LABEL(label_error))) {
        for (int i = 0; username[i]; i++) {
            username[i] = (gchar) tolower(username[i]);
        }
        ///form a json object
        cJSON_AddNumberToObject(j_request, "token", RQ_SIGN_IN);
        cJSON_AddStringToObject(j_request, "username", MX_J_STR(username));
        cJSON_AddStringToObject(j_request, "password", MX_J_STR(password));
        j_data = vm_message_calibration(j_request);
        ///send the request in json format
        vm_send(chat->out, j_data);
    }

    // /freeing memory
    g_free(encrypted_pass);
    cJSON_Delete(j_request);
}

void sign_up_request(t_chat *chat) {
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_signup");
    gchar *username = get_buffer_text("buffer_login", chat->builder);
    gchar *password = get_buffer_text("buffer_password", chat->builder);
    gchar *pass_confirm = get_buffer_text("buffer_password_confirm", chat->builder);
    gchar *encrypted_pass = encrypt_str(password);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_valid_auth_data(username, password, GTK_LABEL(label_error))) {
        if (strcmp(password, pass_confirm) == 0) {
            for (int i = 0; username[i]; i++) {
                username[i] = (gchar) tolower(username[i]);
            }
            ///form a json object
            cJSON_AddNumberToObject(j_request, "token", RQ_SIGN_UP);
            cJSON_AddStringToObject(j_request, "username", MX_J_STR(username));
            cJSON_AddStringToObject(j_request, "password", MX_J_STR(encrypted_pass));
            j_data = vm_message_calibration(j_request);
            ///send the request in json format
            vm_send(chat->out, j_data);
        }
        else {
            gtk_label_set_text(GTK_LABEL(label_error), ERRMSG_DIFPASS);
        }
    }

    ///freeing memory
    g_free(encrypted_pass);
    cJSON_Delete(j_request);
}
