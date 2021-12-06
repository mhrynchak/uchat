#include "client.h"

void delete_selected_msgs(GtkButton *btn, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");

    gtk_list_box_selected_foreach(groom->box_messages, (GtkListBoxForeachFunc)del_msg_request, chat);
    (void)btn;
}

static void prepare_entry_for_edit(GtkBuilder *builder) {
    GObject *btn_stick = gtk_builder_get_object(builder, "btn_choose_sticker");
    GObject *box_stick = gtk_builder_get_object(builder, "box_stickers");
    GObject *msg_entry = gtk_builder_get_object(builder, "msg_entry");

    gtk_widget_grab_focus(GTK_WIDGET(msg_entry));
    gtk_widget_hide(GTK_WIDGET(btn_stick));
    gtk_widget_hide(GTK_WIDGET(box_stick));
}

void show_edit_msg(GtkButton *btn, t_chat *chat) {
    GObject *buffer = gtk_builder_get_object(chat->builder, "buffer_message");
    GObject *label_text = gtk_builder_get_object(chat->builder, "label_edit_text");
    t_gmsg *msg = get_selected_gmsg(chat->builder);
    if(!msg) {
        puts(NULL);
    }
    gchar *old_text = g_strdelimit(g_strdup(msg->msg), "\n", ' ');

    prepare_entry_for_edit(chat->builder);
    chat->msg_placeholder = FALSE;
    gtk_label_set_text(GTK_LABEL(label_text), old_text);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), msg->msg, -1);
    switch_room_header(chat->builder, ROOM_CTRL);
    g_free(old_text);
    (void)btn;
}

void unselect_msg(GtkButton *btn, GtkBuilder *builder) {
    unselect_curr_room_messages(builder);
    switch_room_header(builder, ROOM_CTRL);
    (void)btn;
}
