#include "client.h"

void sig_go_down(GtkButton *btn, GtkBuilder *builder) {
    t_groom *groom = get_selected_groom(builder, "listbox_rooms");
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(groom->page);

    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
    (void)btn;
}
