#include "client.h"

void req_room_sett(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    char *new_name = g_strndup(entry_get_text("entry_roomsett_name",
                                              chat->builder),
                                              VM_MAX_ROOM_NAME);
    char *new_desc = get_buffer_text("buffer_room_desc", chat->builder);

    trim_message(&new_desc);
    if (g_strcmp0(groom->room_name, new_name)) {
        edit_room_name_request(chat->out, groom->id, new_name);
    }
    if (!groom->desc || g_strcmp0(groom->desc, new_desc)) {
        edit_room_desc_request(chat->out, groom->id, new_desc);
    }
    (void)btn;
}

void req_room_name_sett(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    char *new_name = g_strndup(entry_get_text("entry_roomsett_name",
                                              chat->builder),
                                              VM_MAX_ROOM_NAME);
    char *new_desc = get_buffer_text("buffer_room_desc", chat->builder);

    trim_message(&new_desc);
    if (g_strcmp0(groom->room_name, new_name)) {
        edit_room_name_request(chat->out, groom->id, new_name);
    }
    if (!groom->desc || g_strcmp0(groom->desc, new_desc)) {
        edit_room_desc_request(chat->out, groom->id, new_desc);
    }
    (void)btn;
}

void set_room_sett(GtkButton *btn, t_chat *chat) {
    GtkEntry *name = GTK_ENTRY(gtk_builder_get_object(chat->builder, "entry_roomsett_name"));
    GObject *desc = gtk_builder_get_object(chat->builder, "buffer_room_desc");
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(desc), groom->desc, -1);
    gtk_entry_set_text(name, groom->room_name);
    gtk_editable_set_position(GTK_EDITABLE(name), -1);  
    (void)btn;
}

void delete_row_msg(GtkListBoxRow *row, GtkBuilder *builder) {
    if (gtk_list_box_row_is_selected(row))
        switch_room_header(builder, ROOM_CTRL);
    gtk_widget_destroy(GTK_WIDGET(row));
}

t_filter_data *create_filter_data(gchar *search_name) {
    t_filter_data *filter_data = g_malloc(sizeof(t_filter_data));

    filter_data->search_name = search_name;
    filter_data->is_found_rooms = FALSE;
    return filter_data;
}

void set_profile_info(GtkButton *btn, t_chat *chat) {
    GObject *label_login = gtk_builder_get_object(chat->builder, "label_profile_login");
    GObject *label_name = gtk_builder_get_object(chat->builder, "entry_user_name");
    GObject *buff_desc = gtk_builder_get_object(chat->builder, "buffer_profile_desc");

    gtk_label_set_text(GTK_LABEL(label_login), chat->login);
    gtk_entry_set_text(GTK_ENTRY(label_name), chat->name);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buff_desc), chat->desc, -1);
    (void)btn;
}

void insert_room(cJSON *j_room, t_chat *chat) {
    cJSON *room_dup = cJSON_Duplicate(j_room, cJSON_True);
    gint room_id = vm_get_object(room_dup, "id")->valueint;

    cJSON_AddStringToObject(room_dup, "valid", "true");
    cJSON_AddStringToObject(room_dup, "err_msg", "valid roomname");
    new_room_response(room_dup, chat);
    new_messages_request(chat->out, 0, room_id);

    cJSON_Delete(room_dup);
}

void insert_old_msg(cJSON *msg, t_chat *chat, gint room_id) {
    cJSON *dup = cJSON_Duplicate(msg, cJSON_True);
    t_gmsg *gmsg = NULL;

    if (!cJSON_AddNumberToObject(dup, "room_id", room_id)) {
        return;
    }
    gmsg = create_gmsg(dup, chat);
    if (!gmsg) {
        cJSON_Delete(dup);
        return;
    }
    add_message_to_room_old(gmsg, chat);
    cJSON_Delete(dup);
}

void trim_message(gchar **message) {
    gchar *trimed = NULL;

    if (strlen(*message) > MAX_MESSAGE) {
        trimed = g_strndup(*message, MAX_MESSAGE);
        g_free(*message);
        *message = g_strdup(trimed);
        g_free(trimed);
    }
}

void send_message_handle_shift(GtkWidget *textview, GdkEvent *event, t_chat *chat) {
    if (event->key.keyval == SHIFT_KEY)
        chat->shift_hold = false;
    (void)textview;
}

void send_message_handle_enter(GtkTextView *textview, GdkEvent *event, t_chat *chat) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);

    if (event->key.keyval == SHIFT_KEY) {
        chat->shift_hold = true;
    }
    g_usleep(0);
    if (event->key.keyval == ENTER_KEY) {
        if (chat->shift_hold) {
            gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
        }
        else if (!widget_is_visible("box_editing_msg", chat->builder)){
            send_message_request(NULL, chat);
            clear_buffer_text("buffer_message", chat->builder);
        }
        else {
            edit_message_request(NULL, chat);
        }
    }
}

void add_member(char *login, guint64 user_id, guint64 room_id, t_chat *chat) {
    t_groom *groom = get_groom_by_id(room_id, chat->builder);

    if (!groom) {
        return;
    }
    if (g_hash_table_lookup(groom->members, GINT_TO_POINTER(user_id))) {
        return;
    }
    g_hash_table_insert(groom->members, GINT_TO_POINTER(user_id), g_strdup(login));
}

void insert_member(cJSON *member, gint room_id, t_chat *chat) {
    gchar *login = vm_get_valuestring(member, "login");
    guint64 user_id = vm_get_object(member, "id")->valueint;
    t_groom *groom = NULL;

    groom = get_groom_by_id(room_id, chat->builder);

    if (groom->user_id == user_id) {
        groom->customer = g_strdup(login);
    }

    g_hash_table_insert(groom->members,
                        GINT_TO_POINTER(user_id),
                        g_strdup(login));
}

void choose_auth(GtkButton *btn, t_chat *chat) {
    GtkNotebook *note = GTK_NOTEBOOK(gtk_builder_get_object(chat->builder, "notebook_auth"));
    int page_num = gtk_notebook_get_current_page(note);
    if (page_num == 0) {
        sign_in_request(chat);
    }
    else {
        sign_up_request(chat);
    }
    (void)btn;
}

/*
 * Function: encrypt_str
 * -------------------------------
 * returns the encrypted str
 */
gchar *encrypt_str(gchar *str) {
    return g_compute_checksum_for_string(G_CHECKSUM_SHA256, str, strlen(str));   
}

/*
 * Function: match_nsearch
 * -------------------------------
 * Search for a regular expression <regex> in a string <str> of <size> characters
 */
bool match_nsearch(char *str, char *regex, size_t size) {
    char buf[size + 1];
    GRegex *g_reg = NULL;
    GMatchInfo *match_info;
    bool g_res;

    buf[size] = 0;
    g_strlcpy(buf, str, size + 1);
    g_reg = g_regex_new(regex, 0, 0, NULL);
    g_res = g_regex_match(g_reg, buf, 0, &match_info);
    g_match_info_free(match_info);
    return g_res;
}
/*
 * Function: match_search
 * -------------------------------
 * Search for a regular expression <regex> in a string <str>
 */
bool match_search(char *str, char *regex) {
    GRegex *g_reg = NULL;
    GMatchInfo *match_info;
    bool g_res;

    g_reg = g_regex_new(regex, 0, 0, NULL);
    g_res = g_regex_match(g_reg, str, 0, &match_info);
    g_match_info_free(match_info);
    g_regex_unref(g_reg);
    return g_res;
}

bool is_valid_auth_data(char *login, char *pass, GtkLabel *label) {
    if (*login == '\0' || *pass == '\0') {
        gtk_label_set_text(label, ERRMSG_NODATA);
        return false;
    }
    else if (match_search(login, VM_LOGIN_REGEX) == 0) {
        gtk_label_set_text(label, ERRMSG_INVALID_LOGIN);
        return false;
    }
    return true;
}
