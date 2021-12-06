#include "client.h"

static void change_icontheme_paths(GtkBuilder *builder) {
    GObject *desc = gtk_builder_get_object(builder, "buffer_message");
    const gchar *path[] = {"../resources/icons",
                           "../resources/stickers",
                           "../resources/adwaita"};

    gtk_icon_theme_set_search_path(gtk_icon_theme_get_default(), path, 3);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(desc), "Enter your message", -1);
}

gint start_gui(t_chat *chat) {
    change_icontheme_paths(chat->builder);
    gtk_main();
    return 0;
}
