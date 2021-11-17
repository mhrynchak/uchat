#include "client.h"

static void change_icontheme_paths(GtkBuilder *builder) {
    GObject *desc = gtk_builder_get_object(builder, "buffer_message");
    const gchar *path[] = {"../src/gui/resources",
                           "../src/gui/resources/stickers",
                           "../src/gui/resources/Adwaita"};

    gtk_icon_theme_set_search_path(gtk_icon_theme_get_default(), path, 3);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(desc), "Enter your message", -1);
}

gint start_gui(t_chat *chat) {
    change_icontheme_paths(chat->builder);
//    start_main_window(chat); //del
    gtk_main();
    return 0;
}
