#include "client.h"

void sig_clear_history_set_roomname(GtkButton *btn, GtkBuilder *builder) {
    t_groom *groom = get_selected_groom(builder,  "listbox_rooms");
    GObject *label = gtk_builder_get_object(builder,
                                            "label_clear_history_roomname");
    
    gtk_label_set_text(GTK_LABEL(label), groom->room_name);
    (void)btn;
}
