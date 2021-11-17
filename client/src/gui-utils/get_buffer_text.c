#include "client.h"

gchar *get_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buffer = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;

    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),
                                        &start, &end, FALSE);
    }
    else {
        return (gchar*)gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buffer));
    }
}
