#include "client.h"

gboolean sig_stop_search_members(gpointer *entry, gpointer *data, GtkBuilder *builder) {
    GObject *listbox = gtk_builder_get_object(builder, "listbox_info_members");
    GObject *sentry = gtk_builder_get_object(builder, "sentry_messages");

    gtk_entry_set_text(GTK_ENTRY(sentry), "");
    gtk_list_box_set_filter_func(GTK_LIST_BOX(listbox), NULL, NULL, NULL);
    gtk_list_box_invalidate_filter(GTK_LIST_BOX(listbox));
    (void)data;
    (void)entry;
    return FALSE;
}
