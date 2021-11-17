#include "client.h"

GtkBuilder *init_window(gint argc, char **argv) {
/* Run GTK+*/
    gtk_init(&argc, &argv);
    GError* error = NULL;

/* load the file with the interface ui (Glade) */
    GtkBuilder *builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, GUI_PATH, &error) == 0) {
        vm_logger(VM_LOG_FILE, "Gui.glade is missing(xml file).");
        g_critical ("Can't upload file: %s", error->message);
        g_error_free (error);
    }
/* connect signals in automatic mode */
    gtk_builder_connect_signals(builder, builder);
    return builder;
}
