#include "client.h"
#include <stdlib.h>

void clear_room_request(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_DEL_HIST);
    cJSON_AddNumberToObject(j_request, "room_id", groom->id);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    (void)btn;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void edit_room_desc_request(GDataOutputStream *out, gint room_id, gchar *new_desc) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_UPD_ROOM_DESC);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    cJSON_AddStringToObject(j_request, "desc", VM_J_STR(new_desc));
    j_data = vm_message_calibration(j_request);
    vm_send(out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void edit_room_name_request(GDataOutputStream *out, gint room_id, gchar *new_name) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_UPD_ROOM_NAME);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    cJSON_AddStringToObject(j_request, "room_name", VM_J_STR(new_name));
    j_data = vm_message_calibration(j_request);
    vm_send(out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void del_msg_request(GtkListBox *box, GtkListBoxRow *row, t_chat *chat) {
    t_gmsg *gmsg = (t_gmsg*)g_object_get_data(G_OBJECT(row), "gmsg");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_DEL_MSG);
    cJSON_AddNumberToObject(j_request, "room_id", gmsg->room_id);
    cJSON_AddNumberToObject(j_request, "msg_id", gmsg->msg_id);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    (void)box;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void del_room_request(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_DEL_ROOM);
    cJSON_AddNumberToObject(j_request, "room_id", groom->id);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    (void)btn;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void ban_member_request(GObject *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;
    gint user_id = (gint)g_object_get_data(btn, "member_id");

    cJSON_AddNumberToObject(j_request, "token", RQ_BAN_MEMBER);
    cJSON_AddNumberToObject(j_request, "room_id", groom->id);
    cJSON_AddNumberToObject(j_request, "user_id", user_id);

    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void new_messages_request(GDataOutputStream *out, gint date, gint room_id) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_GET_NEW_MSGS);
    cJSON_AddNumberToObject(j_request, "date", date);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    cJSON_AddNumberToObject(j_request, "count", BUF_MSGS);

    j_data = vm_message_calibration(j_request);
    vm_send(out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void join_to_room_request(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_global_rooms");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_JOIN_ROOM);
    cJSON_AddNumberToObject(j_request, "id", groom->id);

    gtk_list_box_unselect_all(groom->box_rooms);
    sig_stop_search_room(NULL, NULL, chat->builder);

    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    (void)btn;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void search_member_request(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search_login = (gchar*)gtk_entry_get_text(GTK_ENTRY(sentry));

    if (strlen(search_login) == 0) {
        sig_stop_search_members(NULL, NULL, chat->builder);
    }
    else {
        search_members(chat->builder, search_login);
    }
}

void search_msg_request(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search_msg = (gchar*)gtk_entry_get_text(GTK_ENTRY(sentry));
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    reset_room_for_search(chat->builder);
    clear_found_msgs(chat->builder);
    if (strlen(search_msg) == 0) {
        sig_stop_search_message(NULL, NULL, chat->builder);
    }
    else {
        t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");

        cJSON_AddNumberToObject(j_request, "token", RQ_SEARCH_MSG);
        cJSON_AddStringToObject(j_request, "search_msg", VM_J_STR(search_msg));
        cJSON_AddNumberToObject(j_request, "room_id", groom->id);
        j_data = vm_message_calibration(j_request);
        vm_send(chat->out, j_data);

        widget_set_visibility_by_name(chat->builder, "stack_message_rooms", false);
        widget_set_visibility_by_name(chat->builder, "scrlldwnd_found_msgs", true);
        widget_set_visibility_by_name(chat->builder, "label_found_messages", true);
        g_free(j_data);
    }

    cJSON_Delete(j_request);
}

void search_room_request(GtkSearchEntry *sentry, t_chat *chat) {
    gchar *search_name = (gchar*)gtk_entry_get_text(GTK_ENTRY(sentry));
    t_filter_data *filter_data = create_filter_data(search_name);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    clear_global_search(chat->builder);
    search_delim_set_visibility(chat->builder, true);
    if (strlen(search_name) == 0) {
        stop_search_room(NULL, NULL, chat->builder);
    }
    else {
        search_local_rooms(chat->builder, filter_data);
        cJSON_AddNumberToObject(j_request, "token", RQ_SEARCH_CH);
        cJSON_AddStringToObject(j_request, "search_name", VM_J_STR(search_name));
        j_data = vm_message_calibration(j_request);
        vm_send(chat->out, j_data);
        g_free(j_data);
    }
    g_free(filter_data);
    cJSON_Delete(j_request);
}

void send_sticker_request(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    GtkImage *image = GTK_IMAGE(gtk_button_get_image(btn));
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;
    gchar *sticker;

    gtk_image_get_icon_name(image, (const gchar**)&sticker, NULL);

    cJSON_AddNumberToObject(j_request, "token", RQ_MSG);
    cJSON_AddStringToObject(j_request, "msg", VM_J_STR(sticker));
    cJSON_AddNumberToObject(j_request, "room_id", groom->id);
    cJSON_AddNumberToObject(j_request, "msg_type", DB_STICKER);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);    
    reset_room_message(NULL, chat->builder);
}

void get_member_info_request(GObject *popup, t_chat *chat) {
    cJSON *j_request = cJSON_CreateObject();
    gint user_id = (gint) g_object_get_data(popup, "member_id");
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_MEMBER_INFO);
    cJSON_AddNumberToObject(j_request, "user_id", user_id);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void edit_user_request(GtkButton *btn, t_chat *chat) {
    gchar *desc = get_buffer_text("buffer_profile_desc", chat->builder);
    gchar *name = g_strdup(entry_get_text("entry_user_name", chat->builder));
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;
    



    trim_message(&desc);
    cJSON_AddNumberToObject(j_request, "token", RQ_UPD_USER);
    cJSON_AddStringToObject(j_request, "desc", VM_J_STR(desc));
    cJSON_AddStringToObject(j_request, "name", VM_J_STR(name));
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);
    
    (void)btn;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void log_out_request(GtkButton *btn, t_chat *chat) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_LOG_OUT);
    cJSON_AddStringToObject(j_request, "auth_token", VM_J_STR(chat->auth_token));
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    (void)btn;
    g_free(j_data);
    cJSON_Delete(j_request);
}

void old_messages_request(GDataOutputStream *out, gint date, gint room_id) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_OLD_MSGS);
    cJSON_AddNumberToObject(j_request, "date", date);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    cJSON_AddNumberToObject(j_request, "count", BUF_MSGS);
    j_data = vm_message_calibration(j_request);
    vm_send(out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void edit_message_request(GtkButton *btn, t_chat *chat) {
    t_gmsg *gmsg = get_selected_gmsg(chat->builder);
    gchar *new_text = get_buffer_text("buffer_message", chat->builder);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    g_strstrip(new_text);
    if (strcmp(gmsg->msg, new_text) && strlen(new_text) > 0) {
        trim_message(&new_text);

        cJSON_AddNumberToObject(j_request, "token", RQ_EDIT_MSG);
        cJSON_AddNumberToObject(j_request, "room_id", gmsg->room_id);
        cJSON_AddStringToObject(j_request, "msg", VM_J_STR(new_text));
        cJSON_AddNumberToObject(j_request, "msg_id", gmsg->msg_id);

        j_data = vm_message_calibration(j_request);
        vm_send(chat->out, j_data);
        g_free(j_data);
    }
    else {
        switch_room_header(chat->builder, MSG_CTRL);
    }
    cJSON_Delete(j_request);
    clear_buffer_text("buffer_message", chat->builder);
    sig_hide_msg_editing(NULL, chat->builder);
    g_free(new_text);
    (void)btn;
}

void send_message_request(GtkButton *btn, t_chat *chat) {
    gchar *message_text = get_buffer_text("buffer_message", chat->builder);
    t_groom *room = get_selected_groom(chat->builder, "listbox_rooms");
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    g_strstrip(message_text);
    if (room && !chat->msg_placeholder && strlen(message_text) > 0) {
        trim_message(&message_text);

        cJSON_AddNumberToObject(j_request, "token", RQ_MSG);
        cJSON_AddNumberToObject(j_request, "room_id", room->id);
        cJSON_AddStringToObject(j_request, "msg", VM_J_STR(message_text));
        cJSON_AddNumberToObject(j_request, "msg_type", DB_TEXT_MSG);
        j_data = vm_message_calibration(j_request);
        vm_send(chat->out, j_data);
        g_free(j_data);
    }
    g_free(message_text);
    cJSON_Delete(j_request);
    clear_buffer_text("buffer_message", chat->builder);
    reset_room_message(NULL, chat->builder);
    (void)btn;
}

void get_members_request(t_chat *chat, gint room_id) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_GET_MEMBERS);
    cJSON_AddNumberToObject(j_request, "room_id", room_id);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void new_room_request(GtkButton *btn, t_chat *chat) {
    gchar *roomname = get_buffer_text("buffer_roomname", chat->builder);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;
    (void)btn;
    cJSON_AddNumberToObject(j_request, "token", RQ_NEW_ROOM);
    cJSON_AddStringToObject(j_request, "name", VM_J_STR(roomname));
    cJSON_AddStringToObject(j_request, "desc", "");
    cJSON_AddNumberToObject(j_request, "type", DB_GLOBAL_CHAT);
    j_data = vm_message_calibration(j_request);
    vm_send(chat->out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
    clear_buffer_text("buffer_roomname", chat->builder);
}

void get_rooms_request(GDataOutputStream *out, gint date) {
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    cJSON_AddNumberToObject(j_request, "token", RQ_GET_ROOMS);
    cJSON_AddNumberToObject(j_request, "date", date);
    j_data = vm_message_calibration(j_request);

    vm_send(out, j_data);

    g_free(j_data);
    cJSON_Delete(j_request);
}

void sign_in_request(t_chat *chat) {
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_login");
    gchar *login = get_buffer_text("buffer_login", chat->builder);
    gchar *pass = get_buffer_text("buffer_password", chat->builder);
    cJSON *j_request = cJSON_CreateObject();
    gchar *encrypted_pass = encrypt_str(pass);
    gchar *j_data = NULL;

    if (is_valid_auth_data(login, pass, GTK_LABEL(label_error))) {
        for (int i = 0; login[i]; i++) {
            login[i] = (gchar) tolower(login[i]);
        }
        ///form a json object
        cJSON_AddNumberToObject(j_request, "token", RQ_SIGN_IN);
        cJSON_AddStringToObject(j_request, "name", "");
        cJSON_AddStringToObject(j_request, "login", VM_J_STR(login));
        cJSON_AddStringToObject(j_request, "pass", VM_J_STR(encrypted_pass));
        cJSON_AddStringToObject(j_request, "desc", "");
        j_data = vm_message_calibration(j_request);
        ///send the request in json format
        vm_send(chat->out, j_data);
        g_free(j_data);
    }

    g_free(encrypted_pass);
    cJSON_Delete(j_request);
}

void sign_up_request(t_chat *chat) {
    GObject *label_error = gtk_builder_get_object(chat->builder, "label_autherror_signup");
    gchar *login = get_buffer_text("buffer_login", chat->builder);
    gchar *pass = get_buffer_text("buffer_password", chat->builder);
    gchar *pass_confirm = get_buffer_text("buffer_password_confirm", chat->builder);
    gchar *encrypted_pass = encrypt_str(pass);
    cJSON *j_request = cJSON_CreateObject();
    gchar *j_data = NULL;

    if (is_valid_auth_data(login, pass, GTK_LABEL(label_error))) {
        if (strcmp(pass, pass_confirm) == 0) {
            for (int i = 0; login[i]; i++) {
                login[i] = (gchar) tolower(login[i]);
            }
            ///form a json object
            cJSON_AddNumberToObject(j_request, "token", RQ_SIGN_UP);
            cJSON_AddStringToObject(j_request, "name", "");
            cJSON_AddStringToObject(j_request, "login", VM_J_STR(login));
            cJSON_AddStringToObject(j_request, "pass", VM_J_STR(encrypted_pass));
            cJSON_AddStringToObject(j_request, "desc", "");
            j_data = vm_message_calibration(j_request);
            ///send the request in json format
            vm_send(chat->out, j_data);
            g_free(j_data);
        }
        else {
            gtk_label_set_text(GTK_LABEL(label_error), ERRMSG_DIFPASS);
        }
    }

    g_free(encrypted_pass);
    cJSON_Delete(j_request);
}
