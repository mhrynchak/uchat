#include "client.h"

gint sort_rooms(GtkListBoxRow *row1, GtkListBoxRow *row2) {
    t_groom *f_room = g_object_get_data(G_OBJECT(row1), "groom");

    if (f_room->is_updated) {
        return FALSE;
    }
    (void)row2;
    return TRUE;
}

static void event_containing(gchar *listbox_name,
                             GtkWidget *event,
                             GtkWidget *label,
                             GtkWidget *row) {
    if (!g_strcmp0(listbox_name, "listbox_rooms")) {
        g_signal_connect(event, "button_press_event", G_CALLBACK(select_room), NULL);
    }
    else {
        g_signal_connect(event, "button_press_event", G_CALLBACK(show_join_to_room), NULL);
    }
    gtk_container_add(GTK_CONTAINER(event), label);
    gtk_container_add(GTK_CONTAINER(row), event);
}

static void fill_room(t_groom *room,
                      GtkListBox *box,
                      GtkWidget *row,
                      GtkWidget *label) {
    room->is_updated = FALSE;
    room->box_rooms = box;
    room->row_room = GTK_LIST_BOX_ROW(row);
    room->label_name = GTK_LABEL(label);
    g_object_ref(box);
    g_object_ref(row);
    g_object_ref(label);
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
    gtk_list_box_set_sort_func(box, (GtkListBoxSortFunc)sort_rooms, NULL, NULL);
    gtk_widget_set_size_request(row, -1, 80);
    gtk_list_box_insert(box, row, -1);
}

void add_room_row(t_groom *room, t_chat *chat, gchar *listbox_name) {
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(chat->builder, listbox_name));
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(room->room_name);
    GtkWidget *event = gtk_event_box_new();
    t_signal_data *data = NULL;

    gtk_widget_set_can_focus(row, FALSE);
    fill_room(room, box, row, label);
    event_containing(listbox_name, event, label, row);
    data = create_sigdata(chat, room, NULL);
    g_object_set_data_full(G_OBJECT(row), "groom", room, (GDestroyNotify)delete_groom);
    g_object_set_data_full(G_OBJECT(event), "sigdata", data, (GDestroyNotify)vm_free_sigdata);
    gtk_widget_show_all(GTK_WIDGET(box));
}
