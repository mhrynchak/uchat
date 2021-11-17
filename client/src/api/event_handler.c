#include "client.h"

void connect_addroom(t_chat *chat) {
    // GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_addroom_apply"));

    if (chat == NULL) {             ///del ///avoids warning of unused variable
        free(chat);
    }

    // g_signal_connect(btn, "clicked", G_CALLBACK(add_room_request), chat);
}

static void set_light_theme(t_chat *chat) {
    g_file_set_contents(THEME_FILE, "light", -1, NULL);
    mx_css_connect(LIGHT_THEME, chat);
    change_theme_icon(chat, "dark-theme");
}

static void enable_opposite_theme(gchar *current_name, t_chat *chat) {
    if (!g_strcmp0("dark", current_name)) {
        g_file_set_contents(THEME_FILE, "light", -1, NULL);
        mx_css_connect(LIGHT_THEME, chat);
        change_theme_icon(chat, "dark-theme");
    }
    else {
        g_file_set_contents(THEME_FILE, "dark", -1, NULL);
        mx_css_connect(DARK_THEME, chat);
        change_theme_icon(chat, "light-theme");
    }
}

void sig_switch_theme(GtkWidget *widget, t_chat *chat) {
    gchar *current_theme = NULL;

    if (g_file_get_contents(THEME_FILE, &current_theme, NULL, NULL)) {
        enable_opposite_theme(current_theme, chat);
    }
    else {
        set_light_theme(chat);
    }
    if (current_theme) {
        g_free(current_theme);
    }
    (void)widget;
}

void connect_theme_switcher(t_chat *chat) {
    GObject *btn = gtk_builder_get_object(chat->builder, "btn_change_theme");

    g_signal_connect(btn, "clicked", G_CALLBACK(sig_switch_theme), chat);
}

void choose_auth(GtkButton *btn, t_chat *chat) {
    GtkNotebook *note = GTK_NOTEBOOK(gtk_builder_get_object(chat->builder, "notebook_auth"));
    int page_num = gtk_notebook_get_current_page(note);
    if (page_num == 0) {
        sign_in_request(chat);
    }
    else {
        sign_up_request(chat);
    }
    (void)btn;
}

void connect_authorization(t_chat *chat) {
    GtkButton *btn_confirm = GTK_BUTTON(gtk_builder_get_object(chat->builder, "btn_auth_confirm"));
    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(choose_auth), chat);
}

