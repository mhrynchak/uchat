#include "client.h"

void clear_room_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    gupd_clear_history(chat->builder, room_id);
}

void edit_room_desc_response(cJSON *j_response, t_chat *chat) {
    gchar *room_desc = vm_get_valuestring(j_response, "desc");
    gint room_id = vm_get_object(j_response, "room_id")->valueint;

    gupd_room_desc(room_id, room_desc, chat->builder);
    g_free(room_desc);
}

void edit_room_name_response(cJSON *j_response, t_chat *chat) {
    gchar *room_name = vm_get_valuestring(j_response, "room_name");
    gint room_id = vm_get_object(j_response, "room_id")->valueint;

    gupd_room_name(room_id, room_name, chat->builder);
    g_free(room_name);
}

void get_member_info_response(cJSON *j_response, t_chat *chat) {
    GObject *login_info = gtk_builder_get_object(chat->builder, "label_user_info_login");
    GObject *name_info = gtk_builder_get_object(chat->builder, "label_user_info_name");
    GObject *desc_info = gtk_builder_get_object(chat->builder, "buffer_user_desc");
    gchar *login = vm_get_valuestring(j_response, "login");
    gchar *name = vm_get_valuestring(j_response, "name");
    gchar *desc = vm_get_valuestring(j_response, "desc");

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(desc_info), desc, -1);
    gtk_label_set_text(GTK_LABEL(name_info), name);
    gtk_label_set_text(GTK_LABEL(login_info), login);
}

void edit_user_response(cJSON *j_response, t_chat *chat) {
    if (chat->desc) {
        g_free(chat->desc);
    }
    if (chat->name) {
        g_free(chat->name);
    }
    chat->desc = vm_get_valuestring(j_response, "desc");
    chat->name = vm_get_valuestring(j_response, "name");
}

void del_msg_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    gint msg_id = vm_get_object(j_response, "msg_id")->valueint;
    t_groom *groom = get_groom_by_id(room_id, chat->builder);
    t_gmsg *gmsg = get_gmsg_by_id(msg_id, room_id, chat->builder);

    if (gmsg && groom) {
        groom->uploaded--;
        delete_row_msg(gmsg->row_msg, chat->builder);
        unselect_curr_room_messages(chat->builder);
    }
}

void del_room_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    t_groom *groom = get_groom_by_id(room_id, chat->builder);
    delete_row_room(groom->row_room, chat->builder);
}

void ban_member_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    gint user_id = vm_get_object(j_response, "user_id")->valueint;
    t_groom *groom = get_groom_by_id(room_id, chat->builder);

    if (g_hash_table_contains(groom->members, GINT_TO_POINTER(user_id))) {
        g_hash_table_remove(groom->members, GINT_TO_POINTER(user_id));
    }
}

void new_member_response(cJSON *j_response, t_chat *chat) {
    gchar *login = vm_get_valuestring(j_response, "login");
    gint id = vm_get_object(j_response, "user_id")->valueint;
    gint room_id = vm_get_object(j_response, "room_id")->valueint;

    add_member(login, id, room_id, chat);
}

void join_to_room_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "id")->valueint;

    new_room_response(j_response, chat);
    new_messages_request(chat->out, 0, room_id);
}

void search_msg_response(cJSON *j_response, t_chat *chat) {
    cJSON *msgs = vm_get_object(j_response, "msgs");
    gint size = cJSON_GetArraySize(msgs);
    t_gmsg *gmsg = NULL;
    cJSON *msg = NULL;

    for (gint i = 0; i < size; i++) {
        msg = cJSON_GetArrayItem(msgs, i);
        gmsg = create_gmsg(msg, chat);
        add_message_to_found(gmsg, chat);
        // delete_gmsg(gmsg);                     ///fix later
    }
    if (size == 0) {
        widget_set_visibility_by_name(chat->builder, "label_search_nothing_msgs", true);
    }
}

void search_room_response(cJSON *j_response, t_chat *chat) {
    cJSON *rooms = vm_get_object(j_response, "rooms");
    gint size = cJSON_GetArraySize(rooms);
    t_groom *groom = NULL;
    cJSON *room = NULL;

    for (gint i = 0; i < size; i++) {
        room = cJSON_GetArrayItem(rooms, i);
        groom = create_groom(room);
        add_room_row(groom, chat, GLOBAL_ROOMS);
        // delete_groom(groom);                     ///fix later
    }
    if (size == 0) {
        widget_set_visibility_by_name(chat->builder, "label_search_nothing_global", true);
    }
}

void log_out_response(cJSON *j_response, t_chat *chat) {
    gchar *auth_token = vm_get_valuestring(j_response, "auth_token");

    if (g_strcmp0(chat->auth_token, auth_token)) {
        g_free(auth_token);
        return;
    }

    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, "listbox_rooms"));
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(box, 0);
    start_main_window(chat);
    while (row) {
        delete_row_room(row, chat->builder);
        row = gtk_list_box_get_row_at_index(box, 0);
    }

    clear_global_search(chat->builder);
}

void new_messages_response(cJSON *j_response, t_chat *chat) {
    cJSON *msgs = vm_get_object(j_response, "messages");
    gint size = cJSON_GetArraySize(msgs);
    cJSON *msg = NULL;

    for (gint i = size - 1; i >= 0; i--) {
        msg = cJSON_GetArrayItem(msgs, i);
        send_message_response(msg, chat);
    }
}

void old_messages_response(cJSON *j_response, t_chat *chat) {
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    cJSON *messages = vm_get_object(j_response, "messages");
    t_groom *groom = NULL;
    cJSON *msg = NULL;

    for (gint i = 0; i < cJSON_GetArraySize(messages); i++) {
        msg = cJSON_GetArrayItem(messages, i);
        insert_old_msg(msg, chat, room_id);
    }
    groom = get_groom_by_id(room_id, chat->builder);
    if (groom == NULL) {
        return;
    }
    groom->uploaded += cJSON_GetArraySize(messages);
    chat->upl_old_msgs = false;
}

void edit_message_response(cJSON *j_response, t_chat *chat) {
    gchar *msg = vm_get_valuestring(j_response, "msg");
    gint room_id = vm_get_object(j_response, "room_id")->valueint;
    gint msg_id = vm_get_object(j_response, "msg_id")->valueint;

    gupd_msg_text(msg_id, room_id, msg, chat->builder);
}

void send_message_response(cJSON *j_response, t_chat *chat) {
    t_gmsg *gmsg = create_gmsg(j_response, chat);
    t_groom *groom = NULL;

    if (gmsg == NULL) {
        return;
    }
    add_message_to_room_new(gmsg, chat);
    groom = get_groom_by_id(gmsg->room_id, chat->builder);
    if (groom == NULL) {
        return;
    }
    if (g_strcmp0(gmsg->login, chat->login)) {
        widget_set_class(GTK_WIDGET(groom->label_name), "has-messages");
    }
    groom->uploaded++;
}

void get_members_response(cJSON *j_response, t_chat *chat) {
    cJSON *members_arr = vm_get_object(j_response, "members");
    cJSON *room_id = vm_get_object(j_response, "room_id");
    gint size = cJSON_GetArraySize(members_arr);
    cJSON *member = NULL;

    for (gint i = 0; i < size; i++) {
        member = cJSON_GetArrayItem(members_arr, i);
        insert_member(member, room_id->valueint, chat);
    }
}

void new_room_response(cJSON *j_response, t_chat *chat) {
    gchar *valid = vm_get_valuestring(j_response, "valid");
    gchar *err_msg = vm_get_valuestring(j_response, "err_msg");
    t_groom *room = NULL;
    
    if (strcmp(valid, "true") == 0) {
        if ((room = create_groom(j_response)) == NULL) {
            return;
        }
        get_members_request(chat, room->id);
        add_messages_box(room, chat);
        add_room_row(room, chat, "listbox_rooms");
    }
    else {
        fprintf(stdout, "%s\n", err_msg);
    }
}

void get_rooms_response(cJSON *j_response, t_chat *chat) {
    cJSON *rooms = vm_get_object(j_response, "rooms");
    gint len = cJSON_GetArraySize(rooms);
    cJSON *room = NULL;

    for (gint i = 0; i < len; i++) {
        room = cJSON_GetArrayItem(rooms, i);
        insert_room(room, chat);
    }
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
        chat->auth_token = vm_get_valuestring(j_response, "auth_token");
        chat->login = vm_get_valuestring(j_response, "login");
        chat->desc = vm_get_valuestring(j_response, "desc");
        chat->name = vm_get_valuestring(j_response, "name");
        get_rooms_request(chat->out, 0);
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
