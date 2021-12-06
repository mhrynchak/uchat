#include "client.h"

void add_message_to_found(t_gmsg *gmsg, t_chat *chat) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *event = create_message_row(chat, gmsg);
    GObject *box = gtk_builder_get_object(chat->builder, "listbox_found_msgs");

    gtk_widget_set_can_focus(row, FALSE);
    gmsg->row_msg = GTK_LIST_BOX_ROW(row);
    g_object_ref(row);
    gtk_container_add(GTK_CONTAINER(row), event);
    gtk_list_box_insert(GTK_LIST_BOX(box), row, -1);
    gtk_widget_show_all(GTK_WIDGET(box));
}

void clear_found_msgs(GtkBuilder *builder) {
    GObject *box = (gtk_builder_get_object(builder, "listbox_found_msgs"));
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);
    
    widget_set_visibility_by_name(builder, "label_search_nothing_msgs", FALSE);
    
    while (row) {
        gtk_widget_destroy(GTK_WIDGET(row));
        row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(box), 0);
    }
    widget_set_visibility_by_name(builder, "stack_message_rooms", TRUE);
    widget_set_visibility_by_name(builder, "scrlldwnd_found_msgs", FALSE);
    widget_set_visibility_by_name(builder, "label_found_messages", FALSE);
}
