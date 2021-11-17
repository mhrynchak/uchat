#include "client.h"

void clear_label_by_name(gchar *label_name, GtkBuilder *builder) {
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, label_name));

    gtk_label_set_text(label, "\0");
}
