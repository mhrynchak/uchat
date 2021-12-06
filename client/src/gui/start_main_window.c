#include "client.h"

void start_main_window(t_chat *chat) {
    GObject *wnd_main = gtk_builder_get_object(chat->builder, "wnd_main");
    GObject *dialog_auth = gtk_builder_get_object(chat->builder, "dialog_auth");

    g_signal_connect(G_OBJECT(wnd_main), "destroy", G_CALLBACK(log_out_request), chat);
    sig_widget_switch_visibility(NULL, GTK_WIDGET(dialog_auth));
    sig_widget_switch_visibility(NULL, GTK_WIDGET(wnd_main));
    sig_reset_auth(NULL, NULL, -1, chat->builder);
}
