#include "client.h"

void connect_theme_switcher(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_change_theme");
    g_signal_connect(btn, "clicked", G_CALLBACK(sig_switch_theme), chat);
}

static void open_file(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    GObject *popup = gtk_builder_get_object(chat->builder, "filechooser");
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(popup));

    reset_room_message(NULL, chat->builder);
    upload_file_request(filename, groom->id, chat);
    g_free(filename);
    (void)btn;
}

void connect_filechooser(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_filechooser_open");

    g_signal_connect(btn, "clicked", G_CALLBACK(open_file), chat);
}

static gboolean unset_placeholder(GtkWidget *textview, GdkEvent  *event, t_chat *chat) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    const gchar *placeholder = "";
    
    if (chat->msg_placeholder) {
        gtk_text_buffer_set_text(buffer, placeholder, -1);
        chat->msg_placeholder = FALSE;
    }
    (void)event;
    return FALSE;
}

void connect_unset_placeholder(t_chat *chat) {
    GObject *textview = gtk_builder_get_object(chat->builder, "msg_entry");

    g_signal_connect(GTK_TEXT_VIEW(textview), "focus-in-event", G_CALLBACK(unset_placeholder), chat);
}

static gboolean set_placeholder(GtkWidget *textview, GdkEvent *event, t_chat *chat) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    const gchar *placeholder = "Enter your message";
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    if (strlen(gtk_text_buffer_get_text(buffer, &start, &end, FALSE)) == 0) {
        gtk_text_buffer_set_text(buffer, placeholder, -1);
        chat->msg_placeholder = TRUE;
    }
    (void)event;
    return FALSE;
}

void connect_set_placeholder(t_chat *chat) {
    GObject *textview = gtk_builder_get_object(chat->builder, "msg_entry");

    g_signal_connect(GTK_TEXT_VIEW(textview), "focus-out-event", G_CALLBACK(set_placeholder), chat);
}

void connect_ban_member(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_ban");

    g_signal_connect(btn, "clicked", G_CALLBACK(ban_member_request), chat);
}

void connect_stickers(t_chat *chat) {
    GObject *sticker_1  = gtk_builder_get_object(chat->builder, "sticker_1");
    GObject *sticker_2  = gtk_builder_get_object(chat->builder, "sticker_2");
    GObject *sticker_3  = gtk_builder_get_object(chat->builder, "sticker_3");
    GObject *sticker_4  = gtk_builder_get_object(chat->builder, "sticker_4");
    GObject *sticker_5  = gtk_builder_get_object(chat->builder, "sticker_5");
    GObject *sticker_6  = gtk_builder_get_object(chat->builder, "sticker_6");
    GObject *sticker_7  = gtk_builder_get_object(chat->builder, "sticker_7");
    GObject *sticker_8  = gtk_builder_get_object(chat->builder, "sticker_8");
    GObject *sticker_9  = gtk_builder_get_object(chat->builder, "sticker_9");
    GObject *sticker_10 = gtk_builder_get_object(chat->builder, "sticker_10");
    GObject *sticker_11 = gtk_builder_get_object(chat->builder, "sticker_11");
    GObject *sticker_12 = gtk_builder_get_object(chat->builder, "sticker_12");
    GObject *sticker_13 = gtk_builder_get_object(chat->builder, "sticker_13");
    GObject *sticker_14 = gtk_builder_get_object(chat->builder, "sticker_14");
    GObject *sticker_15 = gtk_builder_get_object(chat->builder, "sticker_15");
    GObject *sticker_16 = gtk_builder_get_object(chat->builder, "sticker_16");
    GObject *sticker_17 = gtk_builder_get_object(chat->builder, "sticker_17");
    GObject *sticker_18 = gtk_builder_get_object(chat->builder, "sticker_18");
    GObject *sticker_19 = gtk_builder_get_object(chat->builder, "sticker_19");
    GObject *sticker_20 = gtk_builder_get_object(chat->builder, "sticker_20");

    g_signal_connect(sticker_1,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_2,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_3,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_4,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_5,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_6,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_7,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_8,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_9,  "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_10, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_11, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_12, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_13, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_14, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_15, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_16, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_17, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_18, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_19, "clicked", G_CALLBACK(send_sticker_request), chat);
    g_signal_connect(sticker_20, "clicked", G_CALLBACK(send_sticker_request), chat);
}

void connect_join_to_room(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_join");
    
    g_signal_connect(GTK_BUTTON(btn), "clicked", G_CALLBACK(join_to_room_request), chat);
}

void connect_search(t_chat *chat) {
    GObject *sentry_rooms = gtk_builder_get_object(chat->builder, "sentry_rooms");
    GObject *sentry_messages = gtk_builder_get_object(chat->builder, "sentry_messages");
    GObject *sentry_members = gtk_builder_get_object(chat->builder, "sentry_members");

    g_signal_connect(GTK_SEARCH_ENTRY(sentry_rooms), "search-changed", G_CALLBACK(search_room_request), chat);
    g_signal_connect(GTK_SEARCH_ENTRY(sentry_messages), "search-changed", G_CALLBACK(search_msg_request), chat);
    g_signal_connect(GTK_SEARCH_ENTRY(sentry_members), "search-changed", G_CALLBACK(search_member_request), chat);
}

void connect_message_ctrl(t_chat *chat) {
    GObject *btn_delete = gtk_builder_get_object(chat->builder, "btn_delete_msg");
    GObject *btn_edit = gtk_builder_get_object(chat->builder, "btn_edit_msg");
    GObject *btn_unselect = gtk_builder_get_object(chat->builder, "btn_unselect_msg");

    g_signal_connect(btn_delete, "clicked", G_CALLBACK(delete_selected_msgs), chat);
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(show_edit_msg), chat);
    g_signal_connect(btn_unselect, "clicked", G_CALLBACK(unselect_msg), chat->builder);
}

void connect_room_settings(t_chat *chat) {
    GObject *sett = gtk_builder_get_object(chat->builder, "btn_show_room_sett");
    GObject *apply = gtk_builder_get_object(chat->builder, "btn_roomsett_apply");
    GObject *apply_room = gtk_builder_get_object(chat->builder, "btn_roomsett_apply_room");
    GObject *delete = gtk_builder_get_object(chat->builder, "btn_roomsett_delete");
    GObject *clear_ = gtk_builder_get_object(chat->builder, "btn_clear_history");

    g_signal_connect(sett, "clicked", G_CALLBACK(set_room_sett), chat);
    g_signal_connect(apply, "clicked", G_CALLBACK(req_room_sett), chat);
    g_signal_connect(apply_room, "clicked", G_CALLBACK(req_room_name_sett), chat);
    g_signal_connect(delete, "clicked", G_CALLBACK(del_room_request), chat);
    g_signal_connect(clear_, "clicked", G_CALLBACK(clear_room_request), chat);
}

void connect_profile_settings(t_chat *chat) {
    GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_logout"));
    GtkButton *edit = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_edit_profile"));
    GObject *popup = gtk_builder_get_object(chat->builder, "popup_user_info");
    GObject *sett = gtk_builder_get_object(chat->builder, "btn_profile_sett");

    g_signal_connect(btn, "clicked", G_CALLBACK(log_out_request), chat);
    g_signal_connect(edit, "clicked", G_CALLBACK(edit_user_request), chat);
    g_signal_connect(popup, "show", G_CALLBACK(get_member_info_request), chat);
    g_signal_connect(sett, "clicked", G_CALLBACK(set_profile_info), chat);
}

void connect_send_message(t_chat *chat) {
    GObject *btn_send = gtk_builder_get_object(chat->builder, "btn_send_msg");
    GObject *btn_edit = gtk_builder_get_object(chat->builder, "btn_edit_msg_apply");
    GObject *textview = gtk_builder_get_object(chat->builder, "msg_entry");

    g_signal_connect(textview, "key-press-event", G_CALLBACK(send_message_handle_enter), chat);
    g_signal_connect(textview, "key-release-event", G_CALLBACK(send_message_handle_shift), chat);
    g_signal_connect(btn_send, "clicked", G_CALLBACK(send_message_request), chat);
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(edit_message_request), chat);
}

void connect_addroom(t_chat *chat) {
    GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_addroom_apply"));

    g_signal_connect(btn, "clicked", G_CALLBACK(new_room_request), chat);
}

void connect_authorization(t_chat *chat) {
    GtkButton *btn_confirm = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_auth_confirm"));
    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(choose_auth), chat);
}
