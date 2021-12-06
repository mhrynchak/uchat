#include "client.h"

gboolean sig_stop_search_message(gpointer *entry, gpointer *data, GtkBuilder *builder) {
    GObject *sentry = gtk_builder_get_object(builder, "sentry_messages");
    
    gtk_entry_set_text(GTK_ENTRY(sentry), "");
    clear_found_msgs(builder);
    (void)entry;
    (void)data;
    return FALSE;
}
