#include "client.h"

gboolean delete_gmsg(t_gmsg *gmsg) {
    if (gmsg) {
        // vm_free((void**)&(gmsg->msg));       ///uncomment?
        // vm_free((void**)&(gmsg->login));
        vm_free((void**)&gmsg);
        // gmsg = NULL;
    }
    return TRUE;
}

void vm_free_sigdata(t_signal_data *data) {
    vm_free((void **)&data);
}

t_signal_data *create_sigdata(t_chat *chat, t_groom *groom,
                                 GtkListBoxRow *row_msg) {
    t_signal_data *data = vm_malloc(sizeof(t_signal_data));

    data->chat = chat;
    data->groom = groom;
    data->row_msg = row_msg;
    return data;
}

static void add_sigdata(GtkWidget *row, GtkWidget *event, t_chat *chat) {
    t_signal_data *data = NULL;

    gtk_widget_set_can_focus(row, FALSE);
    data = create_sigdata(chat, NULL, GTK_LIST_BOX_ROW(row));
    g_signal_connect(event, "button_release_event", G_CALLBACK(select_msg), data);
    g_object_set_data_full(G_OBJECT(row), "sigdata", data, (GDestroyNotify)vm_free_sigdata);
}

static void add_row(GtkWidget *row, GtkWidget *event, t_gmsg *msg) {
        msg->row_msg = GTK_LIST_BOX_ROW(row);
        g_object_ref(row);
        gtk_container_add(GTK_CONTAINER(row), event);
}

static void add_message_row(t_gmsg *msg, t_chat *chat, gint position) {
    t_groom *groom = get_groom_by_id(msg->room_id, chat->builder);
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *event = create_message_row(chat, msg);
    GtkListBox *box = NULL;

    if (!groom) {
        return;
    }
    box = groom->box_messages;
    add_sigdata(row, event, chat);
    add_row(row, event, msg);

    groom->is_updated = TRUE;
    gtk_list_box_insert(box, row, position);
    gtk_list_box_row_changed(groom->row_room);
    groom->is_updated = FALSE;
    groom->is_watched = FALSE;
    gtk_widget_show_all(GTK_WIDGET(box));
    g_object_set_data_full(G_OBJECT(row), "gmsg", msg, (GDestroyNotify)delete_gmsg);
}

void add_message_to_room_new(t_gmsg *msg, t_chat *chat) {
    add_message_row(msg, chat, -1);
}

void add_message_to_room_old(t_gmsg *msg, t_chat *chat) {
    add_message_row(msg, chat, 0);
}
