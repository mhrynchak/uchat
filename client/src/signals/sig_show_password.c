#include "client.h"

void sig_show_password(GtkEntry *entry, GtkEntryIconPosition icon_pos,
                      GdkEvent *event, gpointer *entry_second) {
    if (gtk_entry_get_visibility(entry)) {
        entry_set_icon_by_path(entry, IMG_EYE, icon_pos);
        gtk_entry_set_visibility(entry, FALSE);
        if (GTK_IS_ENTRY(entry_second)) {
            gtk_entry_set_visibility(GTK_ENTRY(entry_second), FALSE);
        }
    }
    else {
        entry_set_icon_by_path(entry, IMG_CLOSEDEYE, icon_pos);
        gtk_entry_set_visibility(entry, TRUE);
        if (GTK_IS_ENTRY(entry_second)) {
            gtk_entry_set_visibility(GTK_ENTRY(entry_second), TRUE);
        }
    }
    (void)event;
}
