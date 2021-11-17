#include "client.h"

void start_main_window(t_chat *chat) {
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "wnd_main");
    GObject *dialog_auth = gtk_builder_get_object(chat->builder,
                                                  "dialog_auth");

    mx_widget_switch_visibility(NULL, GTK_WIDGET(dialog_auth));
    mx_widget_switch_visibility(NULL, GTK_WIDGET(wnd_main));
    sig_reset_auth(NULL, NULL, -1, chat->builder);
}
