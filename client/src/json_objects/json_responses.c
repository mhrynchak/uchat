//
// Created by Максим Гринчак on 11/4/21.
//
#include "client.h"

void add_room_response(cJSON *j_response, t_chat *chat) {
    gchar *roomname = vm_get_valuestring(j_response, "roomname");
    gchar *valid = vm_get_valuestring(j_response, "valid");

    if (strcmp(valid, "true")) {
        vm_logger(VM_LOG_FILE, "you have created the room successfully!");
    }
    else {
        vm_logger(VM_LOG_FILE, vm_get_valuestring(j_response, "err_msg"));
        ////set error label to "err_msg"
    }

    free(valid);
    free(roomname);
    cJSON_Delete(j_response);
}

void sign_in_response(cJSON *j_response, t_chat *chat) {
    gchar *valid = vm_get_valuestring(j_response, "valid");
    gchar *err_msg = vm_get_valuestring(j_response, "err_msg");
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_login");
    const char *format = "<span color=\"red\">%s</span>";
    char *markup;
    
    if (strcmp(valid, "true") != 0) {
        markup = g_markup_printf_escaped(format, err_msg);
        gtk_label_set_markup(GTK_LABEL(label_error), markup);
        g_free (markup);
    }
    else {
        start_main_window(chat);
    }
}

void sign_up_response(cJSON *j_response, t_chat *chat) {
    gchar *valid = vm_get_valuestring(j_response, "valid");
    gchar *err_msg = vm_get_valuestring(j_response, "err_msg");
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_signup");
    const char *format = "<span color=\"red\">%s</span>";
    char *markup;

    if (strcmp(valid, "true") != 0) {
        markup = g_markup_printf_escaped(format, err_msg);
        gtk_label_set_markup(GTK_LABEL(label_error), markup);
        g_free (markup);
    }
}
