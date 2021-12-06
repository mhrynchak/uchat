#include "client.h"

void gupd_clear_history(GtkBuilder *builder, guint64 room_id) {
    t_groom *groom = get_groom_by_id(room_id, builder);
    GtkListBoxRow *row = NULL;

    if (!groom)
        return;
    row = gtk_list_box_get_row_at_index(groom->box_messages, 0);
    while (row) {
        gtk_widget_destroy(GTK_WIDGET(row));
        row = gtk_list_box_get_row_at_index(groom->box_messages, 0);
        sig_hide_msg_editing(NULL, builder);
        groom->uploaded--;
    }
}

void gupd_room_desc(guint64 id, char *desc, GtkBuilder *builder) {
    t_groom *groom = get_groom_by_id(id, builder);

    if (groom) {
        // free((void **)&(groom->desc));
        groom->desc = strdup(desc);
        set_current_room_sett(builder);
    }
}

void gupd_room_name(guint64 id, char *name, GtkBuilder *builder) {
    t_groom *groom = get_groom_by_id(id, builder);
    
    if (groom) {
        // free((void**)&(groom->room_name));
        groom->room_name = strdup(name);
        gtk_label_set_text(groom->label_name, groom->room_name);
        set_current_room_sett(builder);
    }
}

void gdel_room(guint64 id, GtkBuilder *builder) {
    t_groom *groom = get_groom_by_id(id, builder);
    
    if (groom)
        delete_row_room(groom->row_room, builder);
}
