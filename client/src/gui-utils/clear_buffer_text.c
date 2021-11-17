#include "client.h"

void clear_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buffer = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;

    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start, &end);
    }
    else {
        gtk_entry_buffer_delete_text(GTK_ENTRY_BUFFER(buffer), 0, -1);
    }
}
