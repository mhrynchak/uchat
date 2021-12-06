#include "client.h"

gboolean sig_stop_search_room(gpointer *entry,
                             gpointer *data, GtkBuilder *builder) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    GObject *sentry = gtk_builder_get_object(builder, "sentry_rooms");

    if (!get_selected_groom(builder, "listbox_global_rooms")) {
        search_delim_set_visibility(builder, FALSE);
        gtk_entry_set_text(GTK_ENTRY(sentry), "");
        gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), NULL, NULL, NULL);
        gtk_list_box_invalidate_filter(GTK_LIST_BOX(listbox));
        widget_set_visibility_by_name(builder, "label_search_nothing_local", FALSE);
        clear_global_search(builder);
    }
    (void)entry;
    (void)data;
    return FALSE;
}
