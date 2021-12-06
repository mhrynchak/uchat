#include "client.h"

t_gmsg *get_gmsg_by_id(guint64 msg_id, guint64 room_id, GtkBuilder *builder) {
    t_groom *room = get_groom_by_id(room_id, builder);
    GtkListBox *box = NULL;
    GtkListBoxRow *row = NULL;
    t_gmsg *gmsg = NULL;
    gboolean flag = TRUE;

    if (!room)
        return NULL;
    box = room->box_messages;
    for (gint i = 0; flag; i++) {
        row = gtk_list_box_get_row_at_index(box, i);
        if (row == NULL)
            flag = FALSE;
        else {
            gmsg = (t_gmsg*)g_object_get_data(G_OBJECT(row), "gmsg");
            if (msg_id == gmsg->msg_id)
                return gmsg;
        }
    }
    return NULL;
}

void gupd_msg_text(gint msg_id, gint room_id, char *text, GtkBuilder *builder) {
    t_gmsg *gmsg = get_gmsg_by_id(msg_id, room_id, builder);

    if (gmsg) {
        vm_free((void **)&(gmsg->msg));
        gmsg->msg = vm_strdup(text);
        gtk_label_set_markup(GTK_LABEL(gmsg->label_text), format_text(gmsg->msg));
        gtk_widget_show_all(GTK_WIDGET(gmsg->row_msg));
        unselect_curr_room_messages(builder);
    }
}
